/*=========================================================================
  Program:   OsiriX

  Copyright (c) OsiriX Team
  All rights reserved.
  Distributed under GNU - GPL
  
  See http://www.osirix-viewer.com/copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.
=========================================================================*/

#import <Foundation/Foundation.h>
#import "MyPoint.h"

#import <OpenGL/CGLMacro.h>

enum
{
	ROI_sleep = 0,
	ROI_drawing = 1,
	ROI_selected = 2,
	ROI_selectedModify = 3
};

@class DCMView;
@class DCMPix;
@class StringTexture;
@class DCMObject;

/** \brief Region of Interest
* 
* Region of Interest on a 2D Image:\n
* Types\n
*	tMesure  = line\n
*	tROI = Rectangle\n
*	tOval = Oval\n
*	tOPolygon = Open Polygon\n
*	tCPolygon = Closed Polygon\n
*	tAngle = Angle\n
*	tText = Text\n
*	tArrow = Arrow\n
*	tPencil = Pencil\n
*	t3Dpoint= 3D Point\n
*	t2DPoint = 2D Point\n
*	tPlain = Brush ROI\n
*	tLayerROI = Layer Overlay\n
*	tAxis = Axis\n					
*	tDynAngle = Dynamic Angle\n
*/

@interface ROI : NSObject <NSCoding, NSCopying>
{
	NSLock			*roiLock;
	
	int				textureWidth, textureHeight;

	unsigned char*	textureBuffer;
	
	NSMutableArray *ctxArray;	//All contexts where this texture is used
	NSMutableArray *textArray;	//All texture id

	int				textureUpLeftCornerX,textureUpLeftCornerY,textureDownRightCornerX,textureDownRightCornerY;
	int				textureFirstPoint;
	
	NSMutableArray  *points;
	NSMutableArray  *zPositions;
	NSRect			rect;
	
	long			type;
	long			mode;
	BOOL			needQuartz;
	
	float			thickness;
	
	BOOL			fill;
	float			opacity;
	RGBColor		color;
	
	BOOL			closed,clickInTextBox;
	
	NSString		*name;
	NSString		*comments;
	
	double			pixelSpacingX, pixelSpacingY;
	NSPoint			imageOrigin;
	
	// **** **** **** **** **** **** **** **** **** **** TRACKING
	
	int				PointUnderMouse;
	long			selectedModifyPoint;
	NSPoint			clickPoint, previousPoint, originAnchor;
	long			fontListGL, *fontSize;
	
	DCMView			*curView;
	DCMPix			*pix;
	
	float			rmean, rmax, rmin, rdev, rtotal;
	float			Brmean, Brmax, Brmin, Brdev, Brtotal;
	
	float			mousePosMeasure;
	
	StringTexture	*stringTex;
	NSMutableDictionary	*stanStringAttrib;
	
	ROI*			parentROI;
	
	NSRect			drawRect;
	
	float			offsetTextBox_x, offsetTextBox_y;
	
	char			line1[ 256], line2[ 256], line3[ 256], line4[ 256], line5[ 256], line6[ 256];
	NSString		*textualBoxLine1, *textualBoxLine2, *textualBoxLine3, *textualBoxLine4, *textualBoxLine5, *textualBoxLine6;
	
	BOOL			_displayCalciumScoring;
	int				_calciumThreshold;
	double			_sliceThickness;
	int				_calciumCofactor;
	
	NSString		*layerReferenceFilePath;
	NSImage			*layerImage;//, *layerImageWhenSelected;
	NSData			*layerImageJPEG;//, *layerImageWhenSelectedJPEG;
	float			layerPixelSpacingX, layerPixelSpacingY;
	BOOL			isLayerOpacityConstant;
	BOOL			canColorizeLayer, canResizeLayer;
	NSColor			*layerColor;
	
	NSNumber		*uniqueID;		// <- not saved, only valid during the 'life' of a ROI
	NSTimeInterval	groupID;		// timestamp of a ROI group. Grouped ROI will be selected/deleted together.
	
	BOOL			displayTextualData;
	
	BOOL			locked;
	BOOL			selectable;
	BOOL			isAliased;
	int				originalIndexForAlias;
}

@property NSPoint imageOrigin;
@property(readonly) int textureWidth, textureHeight;
@property(readonly) int textureDownRightCornerX,textureDownRightCornerY, textureUpLeftCornerX, textureUpLeftCornerY;
@property(readonly) unsigned char *textureBuffer;
@property float opacity;
@property int originalIndexForAlias;
@property BOOL locked, selectable, isAliased;
@property(retain) NSString *name, *comments;
@property(readonly) long type;
@property(setter=setROIMode:) long ROImode;
@property(retain) NSMutableArray *points; // Return/set the points state of the ROI
@property(readonly) NSMutableArray *zPositions;
@property(readonly) BOOL clickInTextBox;
@property(setter=setROIRect:) NSRect rect; // To create a Rectangular ROI (tROI) or an Oval ROI (tOval) or a 2DPoint
@property(retain) DCMPix *pix; // The DCMPix associated to this ROI
@property(readonly) DCMView *curView;  // The DCMView associated to this ROI
@property float mousePosMeasure;
@property(readonly) NSData *data;
@property(setter=setColor:) RGBColor rgbcolor;
@property float thickness;
@property(retain) ROI *parentROI;
@property double sliceThickness, pixelSpacingX, pixelSpacingY;
@property float min, max, mean;

- (void) setColor:(RGBColor) a globally: (BOOL) g;
- (void) setThickness:(float) a globally: (BOOL) g;
- (void) setOpacity:(float)newOpacity globally: (BOOL) g;

/** Set default ROI name (if not set, then default name is the currentTool) */
+ (void) setDefaultName:(NSString*) n;
/** Return the default name */
+ (NSString*) defaultName;
+ (void) setFontHeight: (float) f;

- (void) setDefaultName:(NSString*) n;
- (NSString*) defaultName;

+ (BOOL) splineForROI;

/** Load User Defaults */
+(void) loadDefaultSettings;

/** Save User Defaults */
+(void) saveDefaultSettings;

/** Create a new ROI, needs the current pixel resolution and image origin
* @param itype ROI Type
* @param ipixelSpacing  Assumes pixel size is same in both x and y
* @param iimageOrigin  Origin on image
*/
- (id) initWithType: (long) itype :(float) ipixelSpacing :(NSPoint) iimageOrigin;

/** Create a new ROI, needs the current pixel resolution  x and y and image origin* @param itype ROI Type
* @param ipixelSpacingx  Pixel width
* @param ipixelSpacingy  Pixel height
* @param iimageOrigin  Origin on image
*/
- (id) initWithType: (long) itype :(float) ipixelSpacingx :(float) ipixelSpacingy :(NSPoint) iimageOrigin;

/** arg: specific methods for tPlain roi 
* @param tBuff  Pointer to the texture buffer
* @param tWidth Texture width
* @param tHeight Texture height
* @param tName ROI Name
* @param posX Origin.x of texture upper left corner
* @param posY Origin.y of texture upper left corner
* @param ipixelSpacingx  Pixel width
* @param ipixelSpacingy  Pixel height 
* @param iimageOrigin  Origin on image
*/
- (id) initWithTexture: (unsigned char*)tBuff  textWidth:(int)tWidth textHeight:(int)tHeight textName:(NSString*)tName
			 positionX:(int)posX positionY:(int)posY
			  spacingX:(float) ipixelSpacingx spacingY:(float) ipixelSpacingy imageOrigin:(NSPoint) iimageOrigin;



/** Set offset for text box */
- (void) setTextBoxOffset:(NSPoint) o;


/** Prints info about texture to output */
- (void)displayTexture;

/** Set resolution and origin associated to the ROI */
- (void) setOriginAndSpacing :(float) ipixelSpacing :(NSPoint) iimageOrigin;

/** Set resolution and origin associated to the ROI */
- (void) setOriginAndSpacing :(float) ipixelSpacingx :(float) ipixelSpacingy :(NSPoint) iimageOrigin;

/** Set resolution and origin associated to the ROI */
- (void) setOriginAndSpacing :(float) ipixelSpacingx :(float) ipixelSpacingy :(NSPoint) iimageOrigin :(BOOL) sendNotification;

/** Compute the roiArea in cm2 */
- (float) roiArea;

/** Compute the geometric centroid in pixel space */
- (NSPoint) centroid;

/**  Compute the length for tMeasure ROI in cm */
- (float) MesureLength: (float*) pixels;

/**  Compute the length for between two points in cm */
- (float) Length:(NSPoint) mesureA :(NSPoint) mesureB;
- (float) LengthFrom:(NSPoint) mesureA to:(NSPoint) mesureB inPixel: (BOOL) inPixel;

/** Compute an angle between 2 lines */
- (float) Angle:(NSPoint) p2 :(NSPoint) p1 :(NSPoint) p3;

- (float*) dataValuesAsFloatPointer :(long*) no;
- (NSMutableArray*) dataValues;

/** Find a point between two points 
*  @param a First point
*  @param b Second point
*  @param r Weighting bewtween the two points
*/
+ (NSPoint) pointBetweenPoint:(NSPoint) a and:(NSPoint) b ratio: (float) r;


+ (NSMutableArray*) resamplePoints: (NSArray*) points number:(int) no;


/** Update ROI on mouse down. For most rois this will be the origin of the ROI */
- (BOOL)mouseRoiDown:(NSPoint)pt :(int)slice :(float)scale;

/** Update ROI on mouse down in the current image. For most rois this will be the origin of the ROI */
- (BOOL) mouseRoiDown:(NSPoint) pt :(float) scale;

/** Move the ROI */
- (void) roiMove:(NSPoint) offset;

/** Move the ROI */
- (void) roiMove:(NSPoint) offset :(BOOL) sendNotification;

/** Modify roi as mouse is dragged */
- (BOOL) mouseRoiDragged:(NSPoint) pt :(unsigned int) modifier :(float) scale;

/** Moedify roi on mouse up */
- (BOOL) mouseRoiUp:(NSPoint) pt;

/** Returns YES if roi is valid */
- (BOOL) valid;

/** Draw the ROI */
- (void) drawROI :(float) scaleValue :(float) offsetx :(float) offsety :(float) spacingx :(float) spacingy;
- (void) drawROIWithScaleValue:(float)scaleValue offsetX:(float)offsetx offsetY:(float)offsety pixelSpacingX:(float)spacingX pixelSpacingY:(float)spacingY highlightIfSelected:(BOOL)highlightIfSelected thickness:(float)thick prepareTextualData:(BOOL) prepareTextualData;

/** Always returns NO */
- (BOOL) needQuartz;

/** Delete the current selected point */
- (BOOL) deleteSelectedPoint;

/** The info displayed in the text box */
- (NSMutableDictionary*) dataString;

/** Set the font */
- (void) setRoiFont: (long) f :(long*) s :(DCMView*) v;

/** Returns an OpenGL string */
- (void) glStr: (unsigned char *) cstrOut :(float) x :(float) y :(float) line;

/** Recompute */
- (void) recompute;

/** Rotate the ROI */
- (void) rotate: (float) angle :(NSPoint) center;

/** Test to see if ROI can be resized */
- (BOOL)canResize;

/** Resize ROI */
- (void) resize: (float) factor :(NSPoint) center;

/** Test to see if texture can be reduced */
- (BOOL) reduceTextureIfPossible;

/** Merge two brush ROI together */
- (void) mergeWithTexture: (ROI*) r;

/** Add a margin to the buffer */
- (void) addMarginToBuffer: (int) margin;

/** Draw text box for ROI */
- (void) drawTextualData;


/** Test to see if point is in text box or ROI and returns the mode. 
* Can be ROI_Selected or ROI_selectedModify if hit test is YES 
*/
- (long) clickInROI:(NSPoint) pt :(float) offsetx :(float) offsety :(float) scale :(BOOL) testDrawRect;
- (NSPoint) ProjectionPointLine: (NSPoint) Point :(NSPoint) startPoint :(NSPoint) endPoint;

/** Delete texture */
- (void) deleteTexture:(NSOpenGLContext*) c;


/** Set cab resize layer */
- (void) setCanResizeLayer:(BOOL)boo;
// Calcium Scoring
/** Cofactor for Calcium Score 
*	Cofactor values used by Agaston.  
*	Using a threshold of 90 rather than 130. Assuming
*	multislice CT rather than electron beam.
*	We could have a flag for Electron beam rather than multichannel CT
*	and use 130 as a cutoff
*   Based on Hounsfield density of Calcium
*/
- (int)calciumScoreCofactor;
/** Calcium score 
* roi Area * cofactor;  area is is mm2.
*plainArea is number of pixels 
*/
- (float)calciumScore;

/** Calcium volume 
* area * thickness
*/
- (float)calciumVolume;

/** Calcium mass
* Volume * mean CT Density / 250 
 */
- (float)calciumMass;

@property BOOL displayCalciumScoring;
@property int calciumThreshold;

@property(retain) NSString *layerReferenceFilePath;
@property(retain) NSImage *layerImage;
@property float layerPixelSpacingX, layerPixelSpacingY;

- (GLuint)loadLayerImageTexture;
- (void)generateEncodedLayerImage;
- (BOOL)isPoint:(NSPoint)point inRectDefinedByPointA:(NSPoint)pointA pointB:(NSPoint)pointB pointC:(NSPoint)pointC pointD:(NSPoint)pointD;
- (NSPoint)rotatePoint:(NSPoint)point withAngle:(float)alpha aroundCenter:(NSPoint)center;
- (void) displayPointUnderMouse:(NSPoint) pt :(float) offsetx :(float) offsety :(float) scale;

@property(retain) NSString *textualBoxLine1, *textualBoxLine2, *textualBoxLine3, *textualBoxLine4, *textualBoxLine5, *textualBoxLine6;
@property NSTimeInterval groupID;


/** Lower right point of ROI */
- (NSPoint) lowerRightPoint;

@property BOOL isLayerOpacityConstant;
@property BOOL canColorizeLayer;
@property BOOL displayTextualData;
@property(readonly) NSPoint clickPoint;

-(NSMutableArray*)splinePoints;
-(NSMutableArray*)splinePoints:(float) scale;
-(NSMutableArray*)splineZPositions;

@end
