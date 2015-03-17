# ofxNurbs
NURBS in openFrameworks. In development
References: http://www.glprogramming.com/red/chapter12.html
            http://people.cs.vt.edu/yongcao/teaching/cs4204/fall2008/materials/documents/Evaluator_opengl.html

Features:
- The addon is now capable of dealing with videos.
- The nurbs grid resolution can be set to a maximum of 10x10.

ToDo:
- Add xml read and save functionality
- Allow to set each control point's weight
- Replace "void updateImage(ofPixels image)" with a "bool setPixels(ofPtr<ofPixels>)" to improve performance with videos
- Enable position and size handling from outside the addon.
- Allow remote control point handling (through network) and explore more precise ways of controlling them.
