//==============================================================================
function initGL(canvasName) {
	var canvas = document.getElementById(canvasName);

	// Initialize the global variable gl to null.
	gl = null;

	try {
		// Try to grab the standard context. If it fails, fallback to 
		// experimental.
		gl = canvas.getContext("webgl") 
			|| canvas.getContext("experimental-webgl");
	} catch(e) {}

	// If we don't have a GL context, give up now
	if(gl) {
		gl.clearColor(1.0, 0.0, 1.0, 1.0);
		gl.enable(gl.DEPTH_TEST);
		gl.depthFunc(gl.LEQUAL);
		gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	} else {
		alert("Unable to initialize WebGL. Your browser may not support it.");
	}
}

//==============================================================================
function createShader(id) {
	var shaderScript, theSource, currentChild, shader;
   
	shaderScript = document.getElementById(id);

	if(!shaderScript) {
		alert("Shader element not found");
		return null;
	}

	theSource = "";
	currentChild = shaderScript.firstChild;

	while(currentChild) {
		if(currentChild.nodeType == currentChild.TEXT_NODE) {
			theSource += currentChild.textContent;
		}

		currentChild = currentChild.nextSibling;
	}

	if(shaderScript.type == "x-shader/x-fragment") {
		shader = gl.createShader(gl.FRAGMENT_SHADER);
	} else if(shaderScript.type == "x-shader/x-vertex") {
		shader = gl.createShader(gl.VERTEX_SHADER);
	} else {
		// Unknown shader type
		return null;
	}

	gl.shaderSource(shader, theSource);
     
	// Compile the shader program
	gl.compileShader(shader);  

	// See if it compiled successfully
	if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		alert("An error occurred compiling the shaders:\n" 
			+ gl.getShaderInfoLog(shader));  
		return null;  
	}

	return shader;
}

//==============================================================================
function createProgram(vertname, fragname) {
	var prog = gl.createProgram();

	var vert = createShader(vertname);
	var frag = createShader(fragname);

	if(vert == null || frag == null) {
		alert("Error loading shaders");
	}

	gl.attachShader(prog, vert);
	gl.attachShader(prog, frag);
	gl.linkProgram(prog);

	if(!gl.getProgramParameter(prog, gl.LINK_STATUS)) {
		alert("Unable to initialize the shader program: \n" 
			+ gl.getProgramInfoLog(prog));
		return null;
	}

	return prog;
}
