newoption {
    trigger = "gfxapi",
    value = "API",
    description = "Choose a particular 3D API for rendering",
    allowed = {
       { "OpenGL",    "OpenGL" },
       { "DX11",  "DirectX 11 (Windows only)" }
    },
    default = "DX11"
 }