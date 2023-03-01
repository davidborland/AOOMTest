///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        AOOMTest.cpp
//
// Author:      David Borland
//
// Description: Application for testing Ambient Occlusion Opacity Mapping technique
//
///////////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <GL/glew.h>
#include <glut.h>

#include <Camera.h>
#include <Interactor.h>

#include "AOOMGraphics.h"


// Handles object interaction
Interactor* interactor;

// Handles rendering
AOOMGraphics* graphics;

//#define VIDEO


void MouseFunc(int button, int state, int x, int y) {
    bool down = state == GLUT_DOWN;

    if (button == GLUT_LEFT_BUTTON) {
        interactor->LeftButtonEvent(down, x, y);
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        interactor->RightButtonEvent(down, x, y);
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
        interactor->MiddleButtonEvent(down, x, y);
    }
}


void MotionFunc(int x, int y) {
    interactor->MotionEvent(x, y);  
}

void KeyboardFunc(unsigned char key, int x, int y) {
    // Escape to quit
    if (key == 27) {
        delete interactor;
        delete graphics;

        exit(0);
    }

   
    // Background color
    else if (key == '0') {
        graphics->ToggleBackgroundColor();
    }    
    // Diffused ambient occlusion values
    else if (key == '9') {
        graphics->ToggleDiffusion();
    }
    // Toggle texuring
    else if (key == '8') {
        graphics->ToggleTexture();
    }
    // Toggle removing completely enclosed areas
    else if (key == '-') {
        graphics->ToggleRemoveEnclosedAreas();
    }

    
    // Use depth instead of ambient occlusion
    else if (key == '`') {
        graphics->ToggleUseDepth();
    }


    // Render modes
    else if (key == '1') {
        graphics->SetRenderMode(1);
    }
    else if (key == '2') {
        graphics->SetRenderMode(2);
    }
    else if (key == '3') {
        graphics->SetRenderMode(3);
    }    
    else if (key == '4') {
        graphics->SetRenderMode(4);
    }      
    else if (key == '5') {
        graphics->SetRenderMode(5);
    }    
    else if (key == '6') {
        graphics->SetRenderMode(6);
    }
    else if (key == '7') {
        graphics->SetRenderMode(7);
    }

    
    // Per vertex color
    else if (key == 'p') {
        graphics->TogglePerVertexColor();
    }


    // Molecule
    else if (key == 'm') {
        graphics->ToggleMolecule();
    }


    // Blend Opacity
    else if (key == 'k') {
        graphics->DecreaseBlendOpacity();
    }
    else if (key == 'l') {
        graphics->IncreaseBlendOpacity();
    }    
    // AOOM Threshold
    else if (key == 'r') {
        graphics->DecreaseAOOMThreshold();
    }
    else if (key == 't') {
        graphics->IncreaseAOOMThreshold();
    }
    // AOOM Opacity
    else if (key == 'i') {
        graphics->DecreaseAOOMOpacity();
    }
    else if (key == 'o') {
        graphics->IncreaseAOOMOpacity();
    }


    // Halo controls
    else if (key == 'g') {
        graphics->DecreaseAOOMHaloSize();
    }
    else if (key == 'h') {
        graphics->IncreaseAOOMHaloSize();
    }
    else if (key == ',') {
        graphics->DecreaseAOOMHaloSmooth();
    }
    else if (key == '.') {
        graphics->IncreaseAOOMHaloSmooth();
    }


    // Backface opacity
    else if (key == 'b') {
        graphics->ToggleBackfaceOpacity();
    }
    // Backface opacity texture
    else if (key == 'v') {
        graphics->ToggleBackfaceOpacityTexture();
    }


    // Edge highlight
    else if (key == 'e') {
        graphics->ToggleEdgeHighlight();
    }


    // AO color or smoothed AO color
    else if (key == 'c') {
        graphics->ToggleAOColor();
    }


    // Focal distance
    else if (key == 'd') {
        graphics->GetCamera()->SetFocalDistance(graphics->GetCamera()->GetFocalDistance() - 0.5);
    }
    else if (key == 'f') {
        graphics->GetCamera()->SetFocalDistance(graphics->GetCamera()->GetFocalDistance() + 0.5);
    }
    // Eye separation
    else if (key == 'a') {
        graphics->GetCamera()->SetEyeSeparation(graphics->GetCamera()->GetEyeSeparation() - 0.01);
    }
    else if (key == 's') {
        graphics->GetCamera()->SetEyeSeparation(graphics->GetCamera()->GetEyeSeparation() + 0.01);
    }
}

void IdleFunc() {
    glutPostRedisplay();
}

void ReshapeFunc(int w, int h) {
#ifdef VIDEO
    h -= 35;
#endif

    interactor->Reshape(w, h);
    graphics->Reshape(w, h);
    graphics->ResizeTextures(w, h);
}

void DisplayFunc() {
    // Do interaction
    interactor->UpdateRotation();

    graphics->SetTranslation(interactor->GetTranslation());
    graphics->SetRotation(interactor->GetRotation());

    // Render
    graphics->Render();

    glutSwapBuffers();
}


int main(int argc, char* argv[]) {
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
#ifdef VIDEO 
    glutInitWindowSize(640, 525);
#else
    glutInitWindowSize(1024, 1024);
#endif
    glutCreateWindow("Ambient Occlusion Opacity Modulation (AOOM)");


	// Callbacks
    glutIdleFunc(IdleFunc);
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MotionFunc);
    glutKeyboardFunc(KeyboardFunc);

    // Create interactor
    interactor = new Interactor();
    interactor->UseWholeWindow();

    interactor->SetTranslation(Vec3(0.002, 0.029, 0.615)); 

    // Create graphics
    graphics = new AOOMGraphics();
    
    bool useTeleImmersion = false;
    bool useDome = false;

    if (argc <= 1) {
        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/PBG_Deaminase.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/PBG_Synthase.info");        
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/URO_III_Decarboxylase.info"); 
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/URO_III_Synthase.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/Copro_III_Oxidase.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/Copro_III_Oxidase_Monomer.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/Copro_III_Oxidase_Monomer_Section.info");
//        graphics->SetInfoFileName("E:/borland/data/AOOMData/pdb1aua_improved.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/sfh1_PC_PI.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/pdb1aua_improved_color.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/sfh1_PC_PI_color.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/sfh1_PC_PI_clarify.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/sfh1_PC_PI_clarify_FROMVRML.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/sfh1_PC_PI_clarify_TESTBALLS.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/1aua.info");
//        graphics->SetInfoFileName("E:/borland/data/Molecules/AOOMData/1kbb.info");
    }
    else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-TeleImmersion") == 0) {
                useTeleImmersion = true;
            }        
            else if (strcmp(argv[i], "-Dome") == 0) {
                useDome = true;
            }
            else if (strstr(argv[i], ".info") != 0) {
                graphics->SetInfoFileName(std::string(argv[i]));
            }
        }
    }

    // Teleimmersion
    if (useTeleImmersion) {
        glutPositionWindow(0, 0);
        glutReshapeWindow(1920 * 4, 1080 * 2);
//glutReshapeWindow(1024, 512);
        graphics->SetRenderType(Graphics::LeftRightStereo);
        graphics->SetUseRenderType(true);
        interactor->UseHalfWindow();
    }
    else if (useDome) {
        glutPositionWindow(0, 0);
        glutReshapeWindow(1400 * 2, 1050 * 2);
        graphics->SetRenderType(Graphics::Dome);
        graphics->SetUseRenderType(true);
        graphics->SetDarkBackgroundColor(0.5, 0.5, 0.5);
    }


    // Initialize graphics
    if (!graphics->Init()) {
        std::cout << "Graphics initialization failed.  Exiting...\n";
        return -1;
    }

    // Enter glut
    glutMainLoop();

    return 0;
}