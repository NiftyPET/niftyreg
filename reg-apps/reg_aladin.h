/** @file reg_aladin.h
 * @date 20/06/2012
 * @author Marc Modat
 * @brief Header file that contains the string to be returned
 * for the slicer extension of reg_aladin
 */

char xml_aladin[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<executable>\n"
"  <category>Registration.NiftyReg</category>\n"
"  <title>RegAladin (NiftyReg)</title>\n"
"  <description><![CDATA[Module/executable for global registration (rigid and/or affine) based on a block-matching approach and a trimmed least squared optimisation.]]></description>\n"
"  <version>0.0.1</version>\n"
"  <documentation-url>http://cmic.cs.ucl.ac.uk/home/software/</documentation-url>\n"
"  <license>BSD</license>\n"
"  <contributor>Marc Modat, Pankaj Daga, David Cash (UCL)</contributor>\n"
"  <parameters advanced=\"false\">\n"
"    <label>Input images. Reference and floating images are mandatory</label>\n"
"    <description>Input images to perform the registration</description>\n"
"    <image fileExtensions=\".nii,.nii.gz,.nrrd,.png\">\n"
"      <name>referenceImageName</name>\n"
"      <longflag>ref</longflag>\n"
"      <description>Reference image filename (also called Target or Fixed)</description>\n"
"      <label>Reference image</label>\n"
"      <default>required</default>\n"
"      <channel>input</channel>\n"
"    </image>\n"
"    <image fileExtensions=\".nii,.nii.gz,.nrrd,.png\">\n"
"      <name>floatingImageName</name>\n"
"      <longflag>flo</longflag>\n"
"      <description>Floating image filename (also called Source or moving)</description>\n"
"      <label>Floating image</label>\n"
"      <default>required</default>\n"
"      <channel>input</channel>\n"
"    </image>\n"
"    <image fileExtensions=\".nii,.nii.gz,.nrrd,.png\">\n"
"      <name>referenceMaskImageName</name>\n"
"      <longflag>rmask</longflag>\n"
"      <description>Reference mask image filename</description>\n"
"      <label>Ref. mask</label>\n"
"      <default></default>\n"
"      <channel>input</channel>\n"
"    </image>\n"
"    <float>\n"
"      <name>smoothReferenceWidth</name>\n"
"      <longflag>smooR</longflag>\n"
"      <description>Standard deviation in mm (voxel if negative) of the Gaussian kernel used to smooth the reference image</description>\n"
"      <label>Ref .Smooth</label>\n"
"      <default>0</default>\n"
"    </float>\n"
"    <float>\n"
"      <name>smoothFloatingWidth</name>\n"
"      <longflag>smooF</longflag>\n"
"      <description>Standard deviation in mm (voxel if negative) of the Gaussian kernel used to smooth the Floating image</description>\n"
"      <label>Flo. smooth</label>\n"
"      <default>0</default>\n"
"    </float>\n"
"  </parameters>\n"
"  <parameters advanced=\"true\">\n"
"    <label>Input affine parametrisation</label>\n"
"    <description>Optional input affine transformation</description>\n"
"    <file fileExtensions=\".txt,.mat\">\n"
"      <name>inputAffineName</name>\n"
"      <longflag>inaff</longflag>\n"
"      <description>Affine registration matrix stored as a text file</description>\n"
"      <label>Input affine trans. from NiftyReg</label>\n"
"      <default></default>\n"
"      <channel>input</channel>\n"
"    </file>\n"
"  </parameters>\n"
"  <parameters advanced=\"false\">\n"
"    <label>Registration output</label>\n"
"    <description>Final affine trnansformation and warped image</description>\n"
"    <file fileExtensions=\".txt,.mat\">\n"
"      <name>outputAffineFileName</name>\n"
"      <longflag>aff</longflag>\n"
"      <description>Affine registration matrix output, saved as a text file</description>\n"
"      <label>Output affine filename</label>\n"
"      <default>outputAffineResult.txt</default>\n"
"      <channel>output</channel>\n"
"    </file>\n"
"    <image fileExtensions=\".nii,.nii.gz,.nrrd,.png\">\n"
"      <name>outputWarpedImageName</name>\n"
"      <longflag>res</longflag>\n"
"      <description>Warped floating image</description>\n"
"      <label>Output warped image</label>\n"
"      <default>outputAffineResult.nii</default>\n"
"      <channel>output</channel>\n"
"    </image>\n"
"  </parameters>\n"
"  <parameters advanced=\"true\">\n"
"    <label>Various optimisation parameters</label>\n"
"    <description>Various optimisation parameters such as the size of the pyramid or the number of level to use in the pyramidal approach.</description>\n"
"    <integer>\n"
"      <name>levelPyramidNumber</name>\n"
"      <longflag>ln</longflag>\n"
"      <description>Number of level to use to generate the pyramids for the coarse-to-fine approach</description>\n"
"      <label>Level number</label>\n"
"      <default>3</default>\n"
"      <constraints>\n"
"        <minimum>0</minimum>\n"
"        <maximum>10</maximum>\n"
"        <step>1</step>\n"
"      </constraints>\n"
"    </integer>\n"
"    <integer>\n"
"      <name>levelToPerformNumber</name>\n"
"      <longflag>lp</longflag>\n"
"      <description>Number of level to use to run the registration once the pyramids have been created</description>\n"
"      <label>Level to perform</label>\n"
"      <default>3</default>\n"
"      <constraints>\n"
"        <minimum>0</minimum>\n"
"        <maximum>10</maximum>\n"
"        <step>1</step>\n"
"      </constraints>\n"
"    </integer>\n"
"    <integer>\n"
"      <name>iterationNumber</name>\n"
"      <longflag>maxit</longflag>\n"
"      <description>Maximal number of iteration of the trimmed least square approach to perform per total</description>\n"
"      <label>Iteration number</label>\n"
"      <default>5</default>\n"
"      <constraints>\n"
"        <minimum>1</minimum>\n"
"        <maximum>100</maximum>\n"
"        <step>1</step>\n"
"      </constraints>\n"
"    </integer>\n"
"    <float>\n"
"      <name>blockPercentage</name>\n"
"      <longflag>vv</longflag>\n"
"      <description>Percentage of block to use in the optimisation scheme</description>\n"
"      <label>Percentage block</label>\n"
"      <default>50</default>\n"
"      <constraints>\n"
"        <minimum>1</minimum>\n"
"        <maximum>100</maximum>\n"
"        <step>1</step>\n"
"      </constraints>\n"
"    </float>\n"
"    <float>\n"
"      <name>inlierPercentage</name>\n"
"      <longflag>ii</longflag>\n"
"      <description>Percentage of block to consider as inlier in the optimisation scheme</description>\n"
"      <label>Percentage inlier</label>\n"
"      <default>50</default>\n"
"      <constraints>\n"
"        <minimum>1</minimum>\n"
"        <maximum>100</maximum>\n"
"        <step>1</step>\n"
"      </constraints>\n"
"    </float>\n"
"    <boolean>\n"
"      <name>rigidOnly</name>\n"
"      <longflag>rigOnly</longflag>\n"
"      <description>Performs only a rigid registration, rigid then affine by default</description>\n"
"      <label>Rigid only</label>\n"
"      <default>false</default>\n"
"    </boolean>\n"
"    <boolean>\n"
"      <name>affineOnly</name>\n"
"      <longflag>affDirect</longflag>\n"
"      <description>Performs only an affine registration, rigid then affine by default</description>\n"
"      <label>Affine only</label>\n"
"      <default>false</default>\n"
"    </boolean>\n"
"    <boolean>\n"
"      <name>useHeaderOrigin</name>\n"
"      <longflag>nac</longflag>\n"
"      <description>Use the header origin to initialise the transformation. Image centres are used by default</description>\n"
"      <label>Use header</label>\n"
"      <default>false</default>\n"
"    </boolean>\n"
"    <integer-enumeration>\n"
"      <name>interpolation</name>\n"
"      <longflag>interp</longflag>\n"
"      <description>Interpolation order to use internally to warp the floating image</description>\n"
"      <label>Interpolation order</label>\n"
"      <default>1</default>\n"
"      <element>0</element>\n"
"      <element>1</element>\n"
"      <element>3</element>\n"
"    </integer-enumeration>\n"
"  </parameters>\n"
"  <parameters advanced=\"true\">\n"
"    <label>Symmetric registration</label>\n"
"    <description>Options to active a symmetric registration optimisation.</description>\n"
"    <boolean>\n"
"      <name>useSym</name>\n"
"      <longflag>sym</longflag>\n"
"      <description>Performs a symmetric registration where both, forward and backward transformations are optimised</description>\n"
"      <label>Use symmetry</label>\n"
"      <default>false</default>\n"
"    </boolean>\n"
"    <image fileExtensions=\".nii,.nii.gz,.nrrd,.png\">\n"
"      <name>floatingMaskImageName</name>\n"
"      <longflag>fmask</longflag>\n"
"      <description>Floating mask image filename</description>\n"
"      <label>Flo. mask</label>\n"
"      <default></default>\n"
"      <channel>input</channel>\n"
"    </image>\n"
"  </parameters>\n"
"</executable>"
;
