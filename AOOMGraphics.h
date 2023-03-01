///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        AOOMGraphics.h
//
// Author:      David Borland
//
// Description: Class that implements graphical rendering using OpenGL.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef AOOMGRAPHICS_H
#define AOOMGRAPHICS_H


#include <Graphics.h>

#include <OBJObjectAO.h>


class AOOMGraphics : public Graphics {
public:
    AOOMGraphics();
    virtual ~AOOMGraphics();

    void SetInfoFileName(const std::string& fileName);

    void DecreaseBlendOpacity();
    void IncreaseBlendOpacity();

    void DecreaseAOOMThreshold();
    void IncreaseAOOMThreshold();
    void DecreaseAOOMOpacity();
    void IncreaseAOOMOpacity();

    void DecreaseAOOMHaloSize();
    void IncreaseAOOMHaloSize();
    void DecreaseAOOMHaloSmooth();
    void IncreaseAOOMHaloSmooth();

    void ToggleBackfaceOpacity();
    void ToggleBackfaceOpacityTexture();

    void ToggleEdgeHighlight();

    void ToggleUseDepth();

    void SetRenderMode(int mode);
    
    void ToggleMolecule();

    void TogglePerVertexColor();

    void SetDarkBackgroundColor(float r, float g, float b);
    void ToggleBackgroundColor();

    void ToggleDiffusion();
    void ToggleTexture();
    void ToggleRemoveEnclosedAreas();

    void ToggleAOColor();

    virtual void Render();

    void ResizeTextures(int w, int h);

private:
    std::string infoFileName;    

    OBJObjectAO* surface;
    OBJObject* molecule;
    OBJObjectAO* balls;

    GLhandleARB aoomProgramObject;
    GLhandleARB perPixelProgramObject;
    GLhandleARB ballsProgramObject;

	GLhandleARB aoomOpacityProjectionProgramObject;
	GLhandleARB aoomCompositeProgramObject;
	GLhandleARB aoomFilterOpacityProgramObject;
	GLhandleARB verticalBlur;


	GLuint frameBuffer;
	GLuint renderBuffer;

	GLuint texture1;
	GLuint texture2;
	GLuint texture3;
	
	GLenum drawBuffer;

    int stereoCount;
    int windowWidth;
    int windowHeight;

    float blendOpacity;
    float aoomThreshold;
    float aoomPower;

    float aoomHaloSize;
    int aoomHaloSmooth;

    bool backfaceOpacity;
    bool backfaceOpacityTexture;

    bool edgeHighlight;

    bool useDepth;

    int renderMode;
    bool showMolecule;
    bool perVertexColor;

    bool aoColor;

    float darkBackgroundColor[3];
    bool darkBackground;

    bool diffusion;
    bool useTexture;
    bool removeEnclosedAreas;

    // Varying attribute shader locations
    int aoLocation;
    int aoSmoothLocation;

    virtual bool InitGL();
    virtual void Draw();

    bool ParseInfo(std::string& surfaceName,
                   std::string& surfaceAOName,
                   std::string& surfacePVCName,
                   std::string& moleculeName,
                   std::string& ballsName,
                   std::string& ballsPVCName,
                   Quat& surfaceQuat,
                   Quat& moleculeQuat,
                   Vec3& moleculeTrans,
                   Quat& ballsQuat,
                   Vec3& ballsTrans,
                   Vec3& allTrans,
                   double& scale);
};


#endif