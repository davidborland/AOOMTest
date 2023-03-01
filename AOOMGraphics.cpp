///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        AOOMGraphics.cpp
//
// Author:      David Borland
//
// Description: Class that implements graphical rendering using OpenGL.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "AOOMGraphics.h"

#include <math.h>
#include <fstream>

#include <GLSLShader.h>
#include <Utilities.h>


AOOMGraphics::AOOMGraphics() : Graphics() {
    stereoCount = 0;
    windowWidth = 1024;
    windowHeight = 1024;

    blendOpacity = 0.25f;
    aoomThreshold = 0.7f;
    aoomPower = 2.5f;

    aoomHaloSize = 0.05f;
    aoomHaloSmooth = 10;
    
    backfaceOpacity = false;
    backfaceOpacityTexture = false;

    edgeHighlight = false;

    renderMode = 1;
    showMolecule = false;
    perVertexColor = false;

    aoColor = false;

    SetDarkBackgroundColor(0.0, 0.0, 0.0);
    darkBackground = false;

    diffusion = true;
    useTexture = false;
    removeEnclosedAreas = false;

    useDepth = false;

    aoLocation = 0;
    aoSmoothLocation = 0;

    surface = NULL;
}

AOOMGraphics::~AOOMGraphics() {
    delete surface;
    delete molecule;
    delete balls;

	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &renderBuffer);
}


void AOOMGraphics::SetInfoFileName(const std::string& fileName) {
    infoFileName = fileName;
}


void AOOMGraphics::DecreaseBlendOpacity() {
    blendOpacity -= 0.05f;
    blendOpacity = blendOpacity < 0.05f ? 0.05f : blendOpacity;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "blendOpacity"), blendOpacity);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::IncreaseBlendOpacity() {
    blendOpacity += 0.05f;
    blendOpacity = blendOpacity > 1.0f ? 1.0f : blendOpacity;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "blendOpacity"), blendOpacity);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::DecreaseAOOMThreshold() {
    aoomThreshold -= 0.01f;
    aoomThreshold = aoomThreshold < 0.0f ? 0.0f : aoomThreshold;

std::cout << "aoomThreshold = " << aoomThreshold << std::endl;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomThreshold"), aoomThreshold);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomThreshold"), aoomThreshold);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::IncreaseAOOMThreshold() {
    aoomThreshold += 0.01f;
    aoomThreshold = aoomThreshold > 1.0f ? 1.0f : aoomThreshold;

std::cout << "aoomThreshold = " << aoomThreshold << std::endl;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomThreshold"), aoomThreshold);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomThreshold"), aoomThreshold);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::DecreaseAOOMOpacity() {
    aoomPower += 0.1f;
    aoomPower = aoomPower > 20.0f ? 20.0f : aoomPower;

std::cout << "aoomPower = " << aoomPower << std::endl;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomPower"), aoomPower);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomPower"), aoomPower);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::IncreaseAOOMOpacity() {
    aoomPower -= 0.1f;
    aoomPower = aoomPower < 0.0f ? 0.0f : aoomPower;

std::cout << "aoomPower = " << aoomPower << std::endl;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomPower"), aoomPower);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomPower"), aoomPower);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::DecreaseAOOMHaloSize() {
    aoomHaloSize -= 0.005f;
    aoomHaloSize = aoomHaloSize < 0.0f ? 0.0f : aoomHaloSize;

std::cout << "aoomHaloSize = " << aoomHaloSize << std::endl;
}

void AOOMGraphics::IncreaseAOOMHaloSize() {
    aoomHaloSize += 0.005f;
    aoomHaloSize = aoomHaloSize > 1.0f ? 1.0f : aoomHaloSize;

std::cout << "aoomHaloSize = " << aoomHaloSize << std::endl;
}

void AOOMGraphics::DecreaseAOOMHaloSmooth() {
    aoomHaloSmooth -= 1;
    aoomHaloSmooth = aoomHaloSmooth < 1 ? 1 : aoomHaloSmooth;

std::cout << "aoomHaloSmooth = " << aoomHaloSmooth << std::endl;

    glUseProgramObjectARB(aoomFilterOpacityProgramObject);
    glUniform1i(glGetUniformLocation(aoomFilterOpacityProgramObject, "haloSmoothRadius"), aoomHaloSmooth);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(verticalBlur);
    glUniform1i(glGetUniformLocation(verticalBlur, "haloSmoothRadius"), aoomHaloSmooth);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::IncreaseAOOMHaloSmooth() {
    aoomHaloSmooth += 1;
    aoomHaloSmooth = aoomHaloSmooth > 100 ? 100 : aoomHaloSmooth;

std::cout << "aoomHaloSmooth = " << aoomHaloSmooth << std::endl;

    glUseProgramObjectARB(aoomFilterOpacityProgramObject);
    glUniform1i(glGetUniformLocation(aoomFilterOpacityProgramObject, "haloSmoothRadius"), aoomHaloSmooth);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(verticalBlur);
    glUniform1i(glGetUniformLocation(verticalBlur, "haloSmoothRadius"), aoomHaloSmooth);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::ToggleEdgeHighlight() {
    edgeHighlight = !edgeHighlight;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "edgeHighlight"), edgeHighlight);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1i(glGetUniformLocation(aoomOpacityProjectionProgramObject, "edgeHighlight"), edgeHighlight);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::ToggleBackfaceOpacity() {
    backfaceOpacity = !backfaceOpacity;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "backfaceOpacity"), backfaceOpacity);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::ToggleBackfaceOpacityTexture() {
    backfaceOpacityTexture = !backfaceOpacityTexture;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "backfaceOpacityTexture"), backfaceOpacityTexture);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::ToggleUseDepth() {
    useDepth = !useDepth;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "useDepth"), useDepth);
    glUseProgramObjectARB(0);
}



void AOOMGraphics::SetRenderMode(int mode) {
    renderMode = mode;

    std::vector<std::string> dummy;
    std::vector<std::string> defines;

    // Render modes
    if (renderMode == 1) {
        // No defines
    }
    else if (renderMode == 2) {
        defines.push_back("BLENDED_OPACITY");
    }
    else if (renderMode == 3) {
        defines.push_back("AO_ONLY");
    }
    else if (renderMode == 4) {
        defines.push_back("AO_LIGHTING");
    }
    else if (renderMode == 5) {
        defines.push_back("AO_BLENDED_OPACITY");
    }
    else if (renderMode == 6) {
        defines.push_back("AOOM");
    }
    else if (renderMode == 7) {
        defines.push_back("AOOM");
        defines.push_back("AOOM_COLOR");
    }
     
    // Per vertex color
    if (perVertexColor) {
        defines.push_back("PER_VERTEX_COLOR");
    }

    // Load the shaders
    if (!GLSLShader::LoadShaders("AOOM_vp.glsl", "AOOM_fp.glsl", aoomProgramObject, dummy, defines)) {
        std::cout << "AOOMGraphics::SetRenderMode() : Couldn't load shaders" << std::endl;
        return;
    }

	if (!GLSLShader::LoadShaders("AOOMOpacityProjection_vp.glsl", "AOOMOpacityProjection_fp.glsl", aoomOpacityProjectionProgramObject)) {
        std::cout << "AOOMGraphics::SetRenderMode() : Couldn't load shaders" << std::endl;
        return;
    }

	if (!GLSLShader::LoadShaders("AOOMComposite_vp.glsl", "AOOMComposite_fp.glsl", aoomCompositeProgramObject)) {
        std::cout << "AOOMGraphics::SetRenderMode() : Couldn't load shaders" << std::endl;
        return;
    }

	if (!GLSLShader::LoadShaders("AOOMFilterOpacity_vp.glsl", "AOOMFilterOpacity_fp.glsl", aoomFilterOpacityProgramObject)) {
        std::cout << "AOOMGraphics::SetRenderMode() : Couldn't load shaders" << std::endl;
        return;
    }

	if (!GLSLShader::LoadShaders("verticalBlur_vp.glsl", "verticalBlur_fp.glsl", verticalBlur)) {
        std::cout << "AOOMGraphics::SetRenderMode() : Couldn't load shaders" << std::endl;
        return;
    }

    // Set parameters
    glUseProgramObjectARB(aoomProgramObject);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "blendOpacity"), blendOpacity);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomThreshold"), aoomThreshold);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "aoomPower"), aoomPower);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "edgeHighlight"), edgeHighlight);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "backfaceOpacity"), backfaceOpacity);
    glUniform1f(glGetUniformLocation(aoomProgramObject, "backfaceOpacityTexture"), backfaceOpacityTexture);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "useTexture"), useTexture);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "removeEnclosedAreas"), removeEnclosedAreas);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "useDepth"), useDepth);

    int aoOpacityLocation = glGetAttribLocation(aoomProgramObject, "aoOpacityValue");
    int aoColorLocation = glGetAttribLocation(aoomProgramObject, "aoColorValue");

    if (surface) surface->SetAOVariableLocations(aoOpacityLocation, aoColorLocation);


	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomThreshold"), aoomThreshold);
    glUniform1f(glGetUniformLocation(aoomOpacityProjectionProgramObject, "aoomPower"), aoomPower);
    glUniform1i(glGetUniformLocation(aoomOpacityProjectionProgramObject, "edgeHighlight"), edgeHighlight);
    glUniform1i(glGetUniformLocation(aoomOpacityProjectionProgramObject, "removeEnclosedAreas"), removeEnclosedAreas);

    glUseProgramObjectARB(aoomFilterOpacityProgramObject);
    glUniform1i(glGetUniformLocation(aoomFilterOpacityProgramObject, "haloSmoothRadius"), aoomHaloSmooth);

	glUseProgramObjectARB(verticalBlur);
    glUniform1i(glGetUniformLocation(verticalBlur, "haloSmoothRadius"), aoomHaloSmooth);

    glUseProgramObjectARB(0);

    renderMode = mode;
}


void AOOMGraphics::ToggleMolecule() {
    showMolecule = !showMolecule;
}


void AOOMGraphics::TogglePerVertexColor() {
    perVertexColor = !perVertexColor;
    SetRenderMode(renderMode);
}


void AOOMGraphics::SetDarkBackgroundColor(float r, float g, float b) {
    darkBackgroundColor[0] = r;
    darkBackgroundColor[1] = g;
    darkBackgroundColor[2] = b;
}


void AOOMGraphics::ToggleBackgroundColor() {
    darkBackground = !darkBackground;
}


void AOOMGraphics::ToggleDiffusion() {
    diffusion = !diffusion;
    
    if (diffusion) surface->DiffusionOn();
    else surface->DiffusionOff();
}

void AOOMGraphics::ToggleTexture() {
    useTexture = !useTexture;
    
    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "useTexture"), useTexture);
    glUseProgramObjectARB(0);
}

void AOOMGraphics::ToggleRemoveEnclosedAreas() {
    removeEnclosedAreas = !removeEnclosedAreas;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "removeEnclosedAreas"), removeEnclosedAreas);
    glUseProgramObjectARB(0);

	glUseProgramObjectARB(aoomOpacityProjectionProgramObject);
    glUniform1i(glGetUniformLocation(aoomOpacityProjectionProgramObject, "removeEnclosedAreas"), removeEnclosedAreas);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::ToggleAOColor() {
    aoColor = !aoColor;

    glUseProgramObjectARB(aoomProgramObject);
    glUniform1i(glGetUniformLocation(aoomProgramObject, "aoColor"), aoColor);
    glUseProgramObjectARB(0);
}


void AOOMGraphics::Render() {
    surface->SetPosition(translation);
    surface->SetQuaternion(quaternion);

    camera->SetView();
    surface->DepthSort();
    balls->DepthSort();

    Graphics::Render();
}


void AOOMGraphics::ResizeTextures(int w, int h) {    
	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture2);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture3);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    windowWidth = w;
    windowHeight = h;
}



bool AOOMGraphics::InitGL() {
    // Set up OpenGL
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = {5.0, 10.0, 20.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

//    glEnable(GL_RESCALE_NORMAL);
    
    // Toggle, so that toggle toggles it back!
    darkBackground = !darkBackground;
    ToggleBackgroundColor();


	// Render to texture
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture2);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture3);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

//	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);


	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
/*
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
*/	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, texture1, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, texture2, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, texture3, 0);

//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "AOOMGraphics::InitGL() : Framebuffer object error!" << std::endl;
		return false;	
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Load shaders
    SetRenderMode(renderMode);

    if (!GLSLShader::LoadShaders("perPixelLighting_vp.glsl", "perPixelLighting_fp.glsl", perPixelProgramObject)) {
        std::cout << "AOOMGraphics::InitGL() : Couldn't load shaders" << std::endl;
        return false;
    }
    if (!GLSLShader::LoadShaders("balls_vp.glsl", "balls_fp.glsl", ballsProgramObject)) {
        std::cout << "AOOMGraphics::InitGL() : Couldn't load shaders" << std::endl;
        return false;
    }


    // Load surface and molecule
    std::string surfaceName;
    std::string surfaceAOName;
    std::string surfacePVCName;
    std::string moleculeName;
    std::string ballsName;
    std::string ballsPVCName;
    Quat surfaceQuat;
    Quat moleculeQuat;
    Vec3 moleculeTrans;
    Quat ballsQuat;
    Vec3 ballsTrans;
    Vec3 allTrans;
    double scale;

    if (!ParseInfo(surfaceName,
                   surfaceAOName,
                   surfacePVCName,
                   moleculeName,
                   ballsName,
                   ballsPVCName,
                   surfaceQuat,
                   moleculeQuat,
                   moleculeTrans,
                   ballsQuat,
                   ballsTrans,
                   allTrans,
                   scale)) {
        std::cout << "AOOMGraphics::InitGL() : Couldn't load info file " << infoFileName << std::endl;
        return false;
   }

    surface = new OBJObjectAO();
    if (!surface->LoadObject(surfaceName.c_str())) {
        std::cout << "AOOMGraphics::InitGL() : Couldn't load object" << std::endl;
        return false;
    }
    if (!surface->LoadAO(surfaceAOName.c_str())) {
        std::cout << "AOOMGraphics::InitGL() : Couldn't load ambient occlusion data" << std::endl;
        return false;
    }
    if (surfacePVCName != "") {
        if (!surface->LoadPVC(surfacePVCName.c_str())) {
            std::cout << "AOOMGraphics::InitGL() : Couldn't load per vertex color data" << std::endl;
            return false;
        }
    }
    surface->SetCameraPosition(camera->GetPosition());
    surface->ImmediateModeOn();
    surface->CullFaceOff();  

    surface->RotatePoints(surfaceQuat);
//surface->ScalePoints(scale);

    if (diffusion) surface->DiffusionOn();
    else surface->DiffusionOff();


    surface->SaveVTK(surfaceName + ".vtk");
    surface->SaveOBJMTL(surfaceName + "ForEric");

    surface->CreateTexture3D();

surface->NormalizeBounds();




    // Load Molecule
    molecule = new OBJObject();
    if (moleculeName != "") {
        if (!molecule->LoadObject(moleculeName.c_str())) {
            std::cout << "AOOMGraphics::InitGL() : Couldn't load molecule" << std::endl;
            return false;
        }
    }
    molecule->ImmediateModeOff();
    molecule->CullFaceOn();
    molecule->DepthSortOff();

    molecule->RotatePoints(moleculeQuat);
//    molecule->ScalePoints(scale * (1.0 - 0.0164782));
//    molecule->TranslatePoints(moleculeTrans - Vec3(2.54566, 3.94835, -1.57523) * 0.0164782); 
    molecule->ScalePoints(scale);
    molecule->TranslatePoints(moleculeTrans);


    // Load Balls
    balls = new OBJObjectAO();
    if (ballsName != "") {
        if (!balls->LoadObject(ballsName.c_str())) {
            std::cout << "AOOMGraphics::InitGL() : Couldn't load balls" << std::endl;
            return false;
        }
        if (ballsPVCName != "") {
            if (!balls->LoadPVC(ballsPVCName.c_str())) {
                std::cout << "AOOMGraphics::InitGL() : Couldn't load balls per vertex color data" << std::endl;
                return false;
            }
        }
    }
    balls->SetCameraPosition(camera->GetPosition());
    balls->ImmediateModeOn();
    balls->CullFaceOff();

    balls->RotatePoints(ballsQuat);
    balls->ScalePoints(scale);
    balls->TranslatePoints(ballsTrans);


//    surface->TranslatePoints(allTrans);
//    molecule->TranslatePoints(allTrans);
//    balls->TranslatePoints(allTrans);




    return true;
}

void AOOMGraphics::Draw() {
    bool stereo = false;
	if (useRenderType && (renderType == LeftRightStereo || 
                          renderType == LeftRightDTIStereo)) {
        stereo = true;
    }

/*
    if (stereo) {
        if (stereoCount == 0) {
            glViewport(0, 0, windowWidth / 2, windowHeight);
        }
        else {
            glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
        }
    }
*/

/*
    // Molecule     
    molecule->SetPosition(translation);
    molecule->SetQuaternion(quaternion);

    glUseProgramObjectARB(perPixelProgramObject);

    if (showMolecule) molecule->Render();


    // Balls     
    balls->SetPosition(translation);
    balls->SetQuaternion(quaternion);

    glUseProgramObjectARB(ballsProgramObject);

    balls->Render();
*/





	// Render ambient occlusion and projected opacity to texture




	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferDrawBufferEXT(frameBuffer, GL_COLOR_ATTACHMENT0);

//	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearColor(1.0, 1.0, 1.0, 1.0);
    if (!(stereo && stereoCount == 1)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
    glEnable(GL_BLEND);
//	glDisable(GL_BLEND);
	glBlendEquation(GL_MAX);
    glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

 /*   
    if (stereo) {
        if (stereoCount == 0) {
	        camera->SetLeftView();
        }
        else {
            camera->SetRightView();
        }
    }
    else {
        camera->SetView();
    }
*/

    glPushMatrix();

    glUseProgramObjectARB(aoomOpacityProjectionProgramObject);

	int aoOpacityLocation = glGetAttribLocation(aoomOpacityProjectionProgramObject, "aoOpacityValue");
    int aoColorLocation = glGetAttribLocation(aoomOpacityProjectionProgramObject, "aoColorValue");

    surface->SetAOVariableLocations(aoOpacityLocation, aoColorLocation);

/*
    surface->DilateGeometry(true);
    surface->Render();
    surface->DilateGeometry(false);
*/


    if (stereo) {
	    camera->SetLeftView();    
            
        glPushMatrix();

        surface->ExpandPoints(aoomHaloSize);

        surface->Render();

        surface->ExpandPoints(-2.0 * aoomHaloSize);

	    surface->Render();

        camera->SetRightView();    
            
        glPushMatrix();

        surface->Render();

        surface->ExpandPoints(2.0 * aoomHaloSize);

        surface->Render();

        surface->ExpandPoints(-aoomHaloSize);
    }
    else {
        camera->SetView();    
        
        glPushMatrix();

        surface->ExpandPoints(aoomHaloSize);

        surface->Render();

        surface->ExpandPoints(-2.0 * aoomHaloSize);

	    surface->Render();

        surface->ExpandPoints(aoomHaloSize);
    }


  /*
surface->ExpandPoints(aoomHaloSize);

    surface->Render();

surface->ExpandPoints(-2.0 * aoomHaloSize);

	surface->Render();

surface->ExpandPoints(aoomHaloSize);


    glUseProgramObjectARB(0);

    glPopMatrix();
	
    */

	// Filter projected opacity

	glFramebufferDrawBufferEXT(frameBuffer, GL_COLOR_ATTACHMENT1);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glUseProgramObjectARB(aoomFilterOpacityProgramObject);

	glBegin(GL_QUADS);
		glVertex2f(-1, 1);
		glVertex2f(-1, -1);
		glVertex2f(1, -1);
		glVertex2f(1, 1);
	glEnd();



	
	glFramebufferDrawBufferEXT(frameBuffer, GL_COLOR_ATTACHMENT2);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture2);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glUseProgramObjectARB(verticalBlur);

	glBegin(GL_QUADS);
		glVertex2f(-1, 1);
		glVertex2f(-1, -1);
		glVertex2f(1, -1);
		glVertex2f(1, 1);
	glEnd();



	


	// Render final geometry
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

glDisable(GL_DEPTH_TEST);
//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

 glEnable(GL_BLEND);
//glDisable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (darkBackground) {
        glClearColor(darkBackgroundColor[0], 
                     darkBackgroundColor[1],
                     darkBackgroundColor[2],
                     1.0f);
    }
    else {
        glClearColor(1.0, 1.0, 1.0, 1.0);
    }

    if (!(stereo && stereoCount == 1)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	glBindTexture(GL_TEXTURE_RECTANGLE, texture3);
//	glBindTexture(GL_TEXTURE_RECTANGLE, texture1);


    if (stereo) {
        if (stereoCount == 0) {
	        camera->SetLeftView();
        }
        else {
            camera->SetRightView();
        }
    }
    else {
        camera->SetView();
    }

	glPushMatrix();

	glUseProgramObjectARB(aoomProgramObject);

	aoOpacityLocation = glGetAttribLocation(aoomProgramObject, "aoOpacityValue");
    aoColorLocation = glGetAttribLocation(aoomProgramObject, "aoColorValue");

    surface->SetAOVariableLocations(aoOpacityLocation, aoColorLocation);

    surface->Render();

    glUseProgramObjectARB(0);

	glPopMatrix();


    
    if (stereo) {       
        stereoCount = !stereoCount;
    }





/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glUseProgramObjectARB(aoomCompositeProgramObject);

	glBegin(GL_QUADS);

		glTexCoord2f(0, 1024);
		glVertex2f(-1, 1);

		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);

		glTexCoord2f(1024, 0);
		glVertex2f(1, -1);

		glTexCoord2f(1024, 1024);
		glVertex2f(1, 1);


	glEnd();
*/
}


bool AOOMGraphics::ParseInfo(std::string& surfaceName,
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
                             double& scale) {
    surfaceName = "";
    surfaceAOName = "";
    surfacePVCName = "";
    moleculeName = "";
    ballsName = "";
    ballsPVCName = "";
    scale = 1.0;

    std::ifstream infoFile(infoFileName.c_str());
    if (infoFile.fail()) {
        std::cout << "AOOMGraphics::ParseInfo() : Couldn't open " << infoFileName.c_str() << std::endl;
        return false;
    }

    
    std::string s;
    std::vector<std::string> tokens;
    while (!infoFile.eof()) {
        getline(infoFile, s);
        tokens = Utilities::Tokenize(s, " \n");

        if (tokens.size() <= 1) continue;

        if (tokens[0] == "surfaceName") {
            surfaceName = tokens[1];
        }
        else if (tokens[0] == "surfaceAOName") {
            surfaceAOName = tokens[1];
        }
        else if (tokens[0] == "surfacePVCName") {
            surfacePVCName = tokens[1];
        }
        else if (tokens[0] == "moleculeName") {
            moleculeName = tokens[1];
        }        
        else if (tokens[0] == "ballsName") {
            ballsName = tokens[1];
        }        
        else if (tokens[0] == "ballsPVCName") {
            ballsPVCName = tokens[1];
        }
        else if (tokens[0] == "surfaceQuat") {
            if (tokens.size() != 5) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid surfaceQuat in " << infoFileName.c_str() << std::endl;
                continue;
            }

            surfaceQuat.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()));
        }
        else if (tokens[0] == "moleculeQuat") {
            if (tokens.size() != 5) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid moleculeQuat in " << infoFileName.c_str() << std::endl;
                continue;
            }

            moleculeQuat.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()));
        }
        else if (tokens[0] == "moleculeTrans") {
            if (tokens.size() != 4) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid moleculeTrans in " << infoFileName.c_str() << std::endl;
                continue;
            }
            
            moleculeTrans.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
        }
        else if (tokens[0] == "ballsQuat") {
            if (tokens.size() != 5) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid ballsQuat in " << infoFileName.c_str() << std::endl;
                continue;
            }

            ballsQuat.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()), atof(tokens[4].c_str()));
        }
        else if (tokens[0] == "ballsTrans") {
            if (tokens.size() != 4) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid ballsTrans in " << infoFileName.c_str() << std::endl;
                continue;
            }
            
            ballsTrans.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
        }
        else if (tokens[0] == "allTrans") {
            if (tokens.size() != 4) {
                std::cout << "AOOMGraphics::ParseInfo() : Invalid allTrans in " << infoFileName.c_str() << std::endl;
                continue;
            }

            allTrans.Set(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
        }
        else if (tokens[0] == "scale") {
            scale = atof(tokens[1].c_str());
        }
    }

    return true;
}