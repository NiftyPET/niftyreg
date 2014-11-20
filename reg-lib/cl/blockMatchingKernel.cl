

#define REDUCE reduceCustom

 __inline__ float reduceCustom(float data, const unsigned int tid){
	static __local float sData2[64];

	sData2[tid] = data;
	barrier(CLK_LOCAL_MEM_FENCE);

	if (tid < 32) sData2[tid] += sData2[tid + 32];
	if (tid < 16) sData2[tid] += sData2[tid + 16];
	if (tid < 8) sData2[tid] += sData2[tid + 8];
	if (tid < 4) sData2[tid] += sData2[tid + 4];
	if (tid < 2) sData2[tid] += sData2[tid + 2];
	if (tid == 0) sData2[0] += sData2[1];

	barrier(CLK_LOCAL_MEM_FENCE);
	return sData2[0];
}



__kernel void blockMatchingKernel(__global float *resultPosition, __global float *targetPosition, __global int* mask, __global float* targetMatrix_xyz, uint3 blockDims, unsigned int* definedBlock){

	__local float sResultValues[12 * 12 * 12];

	const bool border = get_group_id(0) == get_num_groups(0) - 1 || get_group_id(1) == get_num_groups(1) - 1 || get_group_id(2) == get_num_groups(2) - 1;

	const unsigned int idz = get_local_id(0) / 16;
	const unsigned int idy = (get_local_id(0) - 16 * idz) / 4;
	const unsigned int idx = get_local_id(0) - 16 * idz - 4 * idy;

	const unsigned int bid = get_group_id(0) + get_num_groups(0) * get_group_id(1) + (get_num_groups(0) * get_num_groups(1)) * get_group_id(2);

	const unsigned int xBaseImage = get_group_id(0) * 4;
	const unsigned int yBaseImage = get_group_id(1) * 4;
	const unsigned int zBaseImage = get_group_id(2) * 4;


	const unsigned int tid = get_local_id(0);//0-blockSize

	const unsigned int xImage = xBaseImage + idx;
	const unsigned int yImage = yBaseImage + idy;
	const unsigned int zImage = zBaseImage + idz;

	const unsigned long imgIdx = xImage + yImage *(c_ImageSize.x) + zImage * (c_ImageSize.x * c_ImageSize.y);
	const bool targetInBounds = xImage < c_ImageSize.x && yImage < c_ImageSize.y && zImage < c_ImageSize.z;

	const int currentBlockIndex = tex1Dfetch(activeBlock_texture, bid);

	if (currentBlockIndex > -1){

		for (int n = -1; n <= 1; n += 1)
		{
			for (int m = -1; m <= 1; m += 1)
			{
				for (int l = -1; l <= 1; l += 1)
				{
					const int x = l * 4 + idx;
					const int y = m * 4 + idy;
					const int z = n * 4 + idz;

					const unsigned int sIdx = (z + 4) * 12 * 12 + (y + 4) * 12 + (x + 4);

					const int xImageIn = xBaseImage + x;
					const int yImageIn = yBaseImage + y;
					const int zImageIn = zBaseImage + z;

					const int indexXYZIn = xImageIn + yImageIn *(c_ImageSize.x) + zImageIn * (c_ImageSize.x * c_ImageSize.y);

					const bool valid = (xImageIn >= 0 && xImageIn < c_ImageSize.x) && (yImageIn >= 0 && yImageIn < c_ImageSize.y) && (zImageIn >= 0 && zImageIn < c_ImageSize.z);
					sResultValues[sIdx] = (valid) ? tex1Dfetch(resultImageArray_texture, indexXYZIn) : nanf("sNaN");

				}
			}
		}

		const float rTargetValue = (targetInBounds) ? tex1Dfetch(targetImageArray_texture, imgIdx) : nanf("sNaN");

		const float targetMean = REDUCE(rTargetValue, tid) / 64;
		const float targetTemp = rTargetValue - targetMean;
		const float targetVar = REDUCE(targetTemp*targetTemp, tid);

		float bestDisplacement[3] = { nanf("sNaN"),0.0f,0.0f };
		float bestCC = 0.0f;

		// iteration over the result blocks
		for (unsigned int n = 1; n < 8; n += 1)
		{
			const bool nBorder = (n < 4 && get_group_id(2) == 0) || (n>4 && get_group_id(2) >= get_num_groups(2) - 2);
			for (unsigned int m = 1; m < 8; m += 1)
			{
				const bool mBorder = (m < 4 && get_group_id(1) == 0) || (m>4 && get_group_id(1) >= get_num_groups(1) - 2);
				for (unsigned int l = 1; l < 8; l += 1)
				{


					const bool lBorder = (l < 4 && get_group_id(0) == 0) || (l>4 && get_group_id(0) >= get_num_groups(0) - 2);

					const unsigned int x = idx + l;
					const unsigned int y = idy + m;
					const unsigned int z = idz + n;

					const unsigned int sIdxIn = z * 144 /*12*12*/ + y * 12 + x;

					const float rResultValue = sResultValues[sIdxIn];
					const bool overlap = isfinite(rResultValue) && targetInBounds;
//					const unsigned int bSize = (nBorder || mBorder || lBorder || border) ? countNans(rResultValue, tid, targetInBounds) : 64;//out
					const unsigned int bSize = (nBorder || mBorder || lBorder || border) ? (unsigned int)REDUCE(overlap?1.0f:0.0f, tid) : 64;//out


					if (bSize > 32 && bSize <= 64){

						const float rChecked = overlap ? rResultValue : 0.0f;
						float newTargetTemp = targetTemp;
						float ttargetvar = targetVar;
						if (bSize < 64){

							const float tChecked = overlap ? rTargetValue : 0.0f;
							const float ttargetMean = REDUCE(tChecked, tid) / bSize;
							newTargetTemp = overlap ? tChecked - ttargetMean : 0.0f;
							ttargetvar = REDUCE(newTargetTemp*newTargetTemp, tid);
						}

						const float resultMean = REDUCE(rChecked, tid) / bSize;
						const float resultTemp = overlap ? rResultValue - resultMean : 0.0f;
						const float resultVar = REDUCE(resultTemp*resultTemp, tid);

						const float sumTargetResult = REDUCE((newTargetTemp)*(resultTemp), tid);
						const float localCC = fabs((sumTargetResult) / sqrtf(ttargetvar*resultVar));


						if (tid == 0 && localCC > bestCC) {
							bestCC = localCC;
							bestDisplacement[0] = l - 4.0f;
							bestDisplacement[1] = m - 4.0f;
							bestDisplacement[2] = n - 4.0f;
						}

					}
				}
			}
		}

		if (tid == 0 && isfinite(bestDisplacement[0])) {

			const unsigned int posIdx = 3 * atomic_add(&(definedBlock[0]), 1);
			resultPosition += posIdx;
			targetPosition += posIdx;

			const float targetPosition_temp[3] = {get_group_id(0)*BLOCK_WIDTH,get_group_id(1)*BLOCK_WIDTH, get_group_id(2)*BLOCK_WIDTH };

			bestDisplacement[0] += targetPosition_temp[0];
			bestDisplacement[1] += targetPosition_temp[1];
			bestDisplacement[2] += targetPosition_temp[2];

			//float  tempPosition[3];
			reg_mat44_mul_cuda(targetMatrix_xyz, targetPosition_temp, targetPosition);
			reg_mat44_mul_cuda(targetMatrix_xyz, bestDisplacement, resultPosition);

		}
	}

}