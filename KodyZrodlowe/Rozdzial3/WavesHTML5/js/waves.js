
/**
Dalej: 2016-01-04
- FXAA: http://uihacker.blogspot.com/2013/03/javascript-antialias-post-processing.html
- onscreen config changer (80% users will use this...)
- config editor (20% of users will use this...)
- http://www.ams.org/journals/mcom/1986-47-176/S0025-5718-1986-0856696-4/
**/	
var renderer = new THREE.WebGLRenderer({ antialias: true });
var aspect = window.innerWidth / window.innerHeight;
var camera = new THREE.PerspectiveCamera( 75, aspect, 0.1, 1000 );
var controls = new THREE.OrbitControls( camera, renderer.domElement );
var canvas;
var W,H;

function init()
{
	canvas = document.getElementById('mycanvas');
	W = canvas.width;
	H = canvas.height;
	poke = 1;
}

$("button").hover(
function(){     $(this).fadeTo(2.5,0.8);  },
function() {	$(this).fadeTo(2.5,1.0);  });

$("#pause").hover(
function(){     $(this).fadeTo(2.5,0.8);  },
function() {	$(this).fadeTo(2.5,1.0);  });

$("#clean").hover(
function(){     $(this).fadeTo(2.5,0.8);  },
function() {	$(this).fadeTo(2.5,1.0);  });

$("#poke").hover(
function(){     $(this).fadeTo(2.5,0.8);  },
function() {	$(this).fadeTo(2.5,1.0);  });

$("a").hover(
function(){     $(this).fadeTo(2.5,0.8);  },
function() {	$(this).fadeTo(2.5,1.0);  });

var pause=0;
function switchPause()
{
	if(pause==0)
	{	pause=1;
		$("#pause").html("&#x2611; PAUSE");
	}
	else
	{
		pause=0;
		$("#pause").html("&#x2610; PAUSE");
	}	
}
$("#pause").click( function(){ switchPause();	} );


var picker=0;
function switchPicker()
{
	if(picker==0)
	{	picker=1;
		$("#picker").html("&#x2611; PICKER");
	}
	else
	{
		picker=0;
		$("#picker").html("&#x2610; PICKER");
	}	
}
$("#picker").click( function(){ switchPicker();	} );

var poke=0;
function switchPoke()
{
	if(poke==0)
	{	poke=1;
		controls.enabled = false;
		$("#poke").html("P<font color=\"#005299\">O</font><font color=\"#28a8e0\">K</font>E");
	}
	else
	{
		poke=0;
		controls.enabled = true;
		$("#poke").html("<font color=\"#666666\">POKE</font>");
	}	
}
//$("#poke").click( function(){ switchPoke();	} );

$("#clean").click( function(){ 		initwave(5);	} );


	var rotate=0;
	
	function pauseonoff()
	{
		if(document.getElementById("pausebutton").checked == true)
			pause=1;
		else
			pause=0;
	}
	/*
	document.getElementById("rotatebutton").checked = false;
	rotateonoff();
	function rotateonoff()
	{
		if(document.getElementById("rotatebutton").checked == true)
			rotate=1;
		else
			rotate=0;
	}
	*/
	window.addEventListener( "keypress", onKeyDown, false );
	function onKeyDown(e)
	{
		//if( e.which == 'a')
			//alert("aaa");
		//alert( e.which );
		var c = String.fromCharCode(e.keyCode || e.charCode);
		//alert(e.charCode);
		//if(c=="1")
		if(e.charCode == 49)	{	initwave(1); }		// "1"
		if(e.charCode == 50)	{	initwave(2); }		// "2"
		if(e.charCode == 51)	{	initwave(3); }		// "3"
		if(e.charCode == 52)	{	initwave(4); }		// "4"
		if(e.charCode == 53)		
		{
				initwave(5);		// POKE
		}
		
	}

	
	window.addEventListener( 'resize', onWindowResize, false );
	var controlWidth = 220;//indow.innerWidth * 0.4;
	
	function onWindowResize()
	{
		camera.aspect = (window.innerWidth-controlWidth) / window.innerHeight;
		camera.updateProjectionMatrix();
		renderer.setSize( window.innerWidth - controlWidth, window.innerHeight );

		if(canvas)
		{
			W = canvas.width;
			H = canvas.height;
		}
	}
	
	
	
      // Javascript will go here.
	var scene = new THREE.Scene();


	//var renderer = new THREE.WebGLRenderer();
	document.body.appendChild( renderer.domElement );

	//renderer.setSize( window.innerWidth, window.innerHeight );
	//renderer.setSize( window.innerWidth - controlWidth, window.innerHeight );
	//camera.aspect = (window.innerWidth-controlWidth);
	camera.position.z = 110;
	//camera.updateProjectionMatrix();
	onWindowResize();
	var i;
	var j;
	var NX = 120;
	var NY = 120;
	var geometry = new THREE.Geometry();
	material = new THREE.MeshPhongMaterial({color: 0xffffff, side: 2, shading: //THREE.SmoothShading});
	THREE.FlatShading});


	geometry = new THREE.PlaneGeometry(100,100,NX-1,NY-1);

	
	
	var plane = new THREE.Mesh( geometry, material );
	scene.add( plane );
	
	plane.rotation.x = 1.9;
	plane.rotation.z = 0.6;
	plane.rotation.y = 0;
	
	var light = new THREE.PointLight( 0xffffff, 1, 100 );
	var light2 = new THREE.PointLight( 0xffeeee, 1, 100 );
	var light3 = new THREE.PointLight( 0xffffff, 1, 100 );

	light.position.set( 0, 50, 30 );
	light2.position.set( -25, 60, 25 );
	light3.position.set( 0, 50, -55 );
	scene.add( light );
	scene.add( light2 );
	scene.add( light3 );

	controls.enableDamping = true;
	controls.dampingFactor = 1.5;
	controls.enableZoom = true;

	controls.noPan = true;
	controls.noZoom = false;
	//controls.minPolarAngle = Math.PI/5.0; // radians
	//controls.maxPolarAngle = Math.PI/1.8; // radians
	//controls.minAzimuthAngle = -Math.PI/4;//- Infinity; // radians
	//controls.maxAzimuthAngle = Math.PI/4;//Infinity; // radians
	
	// create array for state
	var U = new Array(NX);
	var F = new Array(NX);
	var E = new Array(NX);
	for (var i = 0; i < NX; i++) 
	{
		U[i] = new Array(NY);
		F[i] = new Array(NY);
		E[i] = new Array(NY);
	}
	
	var DT = 0.05;   // time dt
	var DX = 0.1;     // spatial dx
	//var FREQ = 0.25;   // pulsating source frequency
	var EPS = 0.333;    // laplace operator
	var C_BND = 1;
	var nums = 0;		// num stored
	var SRCX = [];
	var SRCY = [];
	var SRCF = [];
	var SRCA = [];
	var SRCW = [];
	var SRCN = 0;

function reset() { initwave(nums); }
function initwave(num)
{
	pause=1;
	switchPause();
	nums = num;
	var i,j;
	for(i=0; i<NX; i++)
	for(j=0; j<NY; j++)
	{
		F[i][j] = 0;
		U[i][j] = E[i][j] = 0;
	}
	
	// standard for poke mode 
	// it changes only for num=5 (clean mode)
	//poke=0;
	controls.enabled = true;
	
	// switch between different configs
	switch(num)
	{
		case 1:     // source + a gap
					// place wall with small gap 
			for(i=0; i<NX; i++)
				if( Math.abs(i-NX/2) >= 2 )
				{
					F[i][NY/4] = C_BND;
					F[i][NY/4-1] = C_BND;
				}
					
			for(i=0; i<NX; i++)
				if( Math.abs(i-NX/2) >= 4 )
				if( Math.abs(i-NX/4) >= 3 )
				if( Math.abs(i-(NX-NX/4)) >= 3 )
				{
				    F[i][NY/2] = C_BND;
					F[i][NY/2-1] = C_BND;
				}

			SRCN = 1;
			SRCX[0] = 0.5;
			SRCY[0] = 0.125;
			SRCF[0] = 0.25;
			SRCA[0] = 0.007;	
			SRCW[0] = 500.0;			
		break;

		case 2:     // source in the middle
			SRCN = 1;
			SRCX[0] = 0.5;
			SRCY[0] = 0.5;
			SRCF[0] = 0.25;
			SRCA[0] = 0.004;
			SRCW[0] = 500.0;
		break;

		case 3:     // interference of two sources
	    	SRCN = 2;
			SRCX[0] = 0.25;
			SRCY[0] = 0.25;
			SRCF[0] = 0.32;
			SRCA[0] = 0.007;
			SRCW[0] = 500.0;
			SRCX[1] = 0.75;
			SRCY[1] = 0.75;
			SRCF[1] = 0.35;
			SRCA[1] = 0.007;	
			SRCW[1] = 500.0;
		break;
		
		case 4:     // interference of two sources
			SRCN = (4+6 * Math.random())|0;
			for(var k=0; k<SRCN; k++)
			{
				SRCX[k] = 0.1+0.8*Math.random();
				SRCY[k] = 0.1+0.8*Math.random();
				SRCF[k] = 0.1 + Math.random()*0.32;
				SRCA[k] = 0.002+Math.random()*0.007;
				SRCW[k] = 100+Math.random()*500.0;
			}
			
			break;
		
		case 5:
			SRCN = 0;
			//if(poke==0)
				//switchPoke();
		break;

		
		default:
			break;
	}
}


// idle - simulation step
function simulation()
{
 	var i,j;
	var x,y;

	// solve equation for U
	// dU/dt = nabla^2 E
	for(j=1;j<NY-1;j++)
	for(i=1;i<NX-1;i++)
	{
		U[i][j]=U[i][j]+
		(
		(1-EPS)*(E[i+1][j]+E[i-1][j]+E[i][j-1]+E[i][j+1]-4*E[i][j]) + EPS   * (E[i+1][j+1]+E[i+1][j-1]+E[i-1][j-1]+E[i-1][j+1]-4*E[i][j])
		)*(dt*DT)/(DX*DX);
	}

	// solve equation for E
	// dE/dt = U

	for(j=0;j<NY;j++)
	for(i=0;i<NX;i++)
	{
		E[i][j] = E[i][j] + U[i][j] * (dt*DT);
	}

	// start boundaries: pulsating sources
	
	for(var k=0; k<SRCN; k++)
	for(i=0;i<NX;i++)
	for(j=0;j<NX;j++)
	{
		dx = .95*Math.abs(i/NX-SRCX[k]);// + 0.25*Math.sin(time*0.25);
		dy = .95*Math.abs(j/NY-SRCY[k]);
		U[i][j] -= SRCA[k]*Math.sin( time*SRCF[k] ) *Math.exp(-SRCW[k]*(dx*dx + dy*dy));
	}


	// boundaries

	for(i=0;i<NX;i++)
	for(j=0;j<NX;j++)
		if(F[i][j] == C_BND)
			E[i][j]=U[i][j]=0.0;

	// rigid wall 1
	for(i=0;i<NX;i++)
	{
		U[i][NY]=U[i][NY-1]=0;
		E[i][NY]=E[i][NY-1]=0;

		U[i][0]=U[i][1]=0;
		E[i][0]=E[i][1]=0;
	}

	// rigid wall 2

	for(j=0;j<NY;j++)
	{
		U[0][j]=U[1][j]=0;
		E[0][j]=E[1][j]=0;

		U[NX-1][j]=U[NX-2][j]=0;
		E[NX-1][j]=E[NX-2][j]=0;
	}
	
	
}

initwave(2);
var time = 0;
var dt = 0.4;
	
var raycaster = new THREE.Raycaster();
var mouse = new THREE.Vector2();

var mouse2D = new THREE.Vector2();
var myszka = 0;

var selectedk = 0;		// k selected by mouse
var selectedk_place = 0;		// k selected by mouse
var selectedk_move_place = 0;		// k selected by mouse moving over plane

function onDocumentMouseDown( event ) 
{
		
	if(poke==0)
		return;
 
	event.preventDefault();
		
		
	if(event.which==1) return;
	
	mouse2D.x = ( event.clientX / renderer.domElement.width ) * 2 - 1;
	mouse2D.y = - ( event.clientY / renderer.domElement.height ) * 2 + 1;
   
    var vector = new THREE.Vector3( mouse2D.x, mouse2D.y, camera.near );
	vector.unproject(camera);

    var raycaster = new THREE.Raycaster( camera.position, vector.sub( camera.position ).normalize() );

    var intersects = raycaster.intersectObjects(scene.children);
	
    // Toggle rotation bool for meshes that we clicked
    if ( intersects.length > 0 ) 
	{
        var match = intersects[0];
        var selectedTile = match.faceIndex;
        var f = face = match.face;
        var faceIdx = match.faceIndex;
        var faces = match.object.geometry.faces;
        var vertices = match.object.geometry.vertices;
		
        //plane.geometry.vertices[f.c].setZ( vertices[f.c].z-0.9);
		//plane.geometry.verticesNeedUpdate=true;
		
		// find index
		// closest to vertices[f.a]
		
		var vx = vertices[f.a].x;
		var vy = vertices[f.a].y;
		var vz = vertices[f.a].z;
		
		var k=0;
		var EPS = 5;
		var xij,yij,zij;
		for (var j = 0; j < NY; j++)
		for (var i = 0; i < NX; i++) 
		{
			xij = plane.geometry.vertices[k].x;
			yij = plane.geometry.vertices[k].y;
			zij = plane.geometry.vertices[k].z;
			if( (xij-vx)*(xij-vx) + (yij-vy)*(yij-vy) + (zij-vz)*(zij-vz) < EPS)
				break;
			k++;
		}
		selectedk_place = k;


    }
}

function onDocumentMouseMove( event ) 
{

	if(picker==0) return;
	selectedk_move_place=0;
	if(poke==0)	return;
	event.preventDefault();		
	mouse2D.x = ( event.clientX / renderer.domElement.width ) * 2 - 1;
	mouse2D.y = - ( event.clientY / renderer.domElement.height ) * 2 + 1;
   
    var vector = new THREE.Vector3( mouse2D.x, mouse2D.y, camera.near );
	vector.unproject(camera);

    var raycaster = new THREE.Raycaster( camera.position, vector.sub( camera.position ).normalize() );

    var intersects = raycaster.intersectObjects(scene.children);
	
    // Toggle rotation bool for meshes that we clicked
    if ( intersects.length > 0 ) 
	{
        var match = intersects[0];
        var selectedTile = match.faceIndex;
        var f = face = match.face;
        var faceIdx = match.faceIndex;
        var faces = match.object.geometry.faces;
        var vertices = match.object.geometry.vertices;		
		var vx = vertices[f.a].x;
		var vy = vertices[f.a].y;
		var vz = vertices[f.a].z;
		var k=0;
		var EPS = 5;
		for (var j = 0; j < NY; j++)
		for (var i = 0; i < NX; i++) 
		{
			var xij = plane.geometry.vertices[k].x;
			var yij = plane.geometry.vertices[k].y;
			var zij = plane.geometry.vertices[k].z;
			if( (xij-vx)*(xij-vx) + (yij-vy)*(yij-vy) + (zij-vz)*(zij-vz) < EPS)
				break;
			k++;
		}
		selectedk_move_place = k;
		
    }
}

window.addEventListener( 'mousemove', onDocumentMouseMove, false );
window.addEventListener( 'mousedown', onDocumentMouseDown, false );
//window.addEventListener( 'mouseup', onDocumentMouseUp, false );
//http://threejs.org/docs/#Reference/Core/Raycaster

function findk(i, j)
{
	// unlimited stupidity
	var k=0;
	for (var jj = 0; jj < j; jj++)
	for (var ii = 0; ii < i; ii++) 
	{
		k+=1;
	}
	return k;
}

function updateMarkers()
{
	    var selectedObject = scene.getObjectByName("wskaznik");
		scene.remove( selectedObject );
			
		/*
		for (var j = 0; j < NY; j++)
		for (var i = 0; i < NX; i++) 
		{
			if(k==selectedk_place)
			{
				//alert(k);
				//U[i][j] = E[i][j]+=0.0001;
				SRCX[SRCN] = i/NX;
				SRCY[SRCN] = j/NY;
				SRCF[SRCN] = 0.25;// + Math.random()*0.12;
				SRCA[SRCN] = 0.009;//+Math.random()*0.002;
				SRCW[SRCN] = 10500;//+Math.random()*100.0;
				SRCN++;
				selectedk_place=0;
				break;
			}
			k+=1;
		}
		*/

		var n,i,j,k;
		var vx=0,vy=0,vz=0;
		for(n=0; n<SRCN; n++)
		{
			var geometry = new THREE.BoxGeometry( 1, 1, 12 );
			i = (SRCX[SRCN] * NX)|0;
			j = (SRCY[SRCN] * NY)|0;
			k=findk(i,j);
			vx = plane.geometry.faces[k].x;
			vy = plane.geometry.faces[k].y;
			vz = plane.geometry.faces[k].z;
			
			geometry.translate(vx, vy, vz);
			var material = new THREE.MeshLambertMaterial( { color: 0xFFFF00 } );
			var mesh = new THREE.Mesh( geometry, material );
			mesh.rotation.x = 1.9;
			mesh.rotation.z = 0.6;
			mesh.rotation.y = 0;
			mesh.name = "wskaznik";
			scene.add( mesh );		
		}
		
}

var render = function () 
{
	if(pause==0)
	{
		time = time + dt;
		//plane.rotation.z += 0.00025;
		if(rotate)
		plane.rotation.z += 0.01;
		simulation();
	}

	k=0;
	var maxl = plane.geometry.vertices.length;
	var RESCALE = 150.0;
	var k=0;
	for (var j = 0; j < NY; j++)
	for (var i = 0; i < NX; i++) 
	{
		if(k<maxl)
			plane.geometry.vertices[k].z = RESCALE*U[i][j];
			
		if(F[i][j] == C_BND)
		{
			//plane.geometry.vertices[k].z = 0;//-4;
			plane.geometry.vertices[k].z = -5;//-4;
			//plane.geometry.faces[k].vertexColors[0] = new THREE.Color(0xFF0000);;//-4;
		}
		k+=1;
	}

	// user mouse select k
	k=0;
	if(selectedk_place != 0)
	{
		for (var j = 0; j < NY; j++)
		for (var i = 0; i < NX; i++) 
		{
			if(k==selectedk_place)
			{
				//alert(k);
				//U[i][j] = E[i][j]+=0.0001;
				SRCX[SRCN] = i/NX;
				SRCY[SRCN] = j/NY;
				SRCF[SRCN] = 0.25;// + Math.random()*0.12;
				SRCA[SRCN] = 0.009;//+Math.random()*0.002;
				SRCW[SRCN] = 10500;//+Math.random()*100.0;
				SRCN++;
				selectedk_place=0;
				break;
			}
			k+=1;
		}
		
	
		updateMarkers();
	
	}


	if(selectedk_move_place != 0)
	for (var j = 0; j < NY; j++)
	for (var i = 0; i < NX; i++) 
	{
	
		if(k==selectedk_move_place)
		{
			plane.geometry.vertices[k].z = -125;//-4;			
		}	
		k+=1;
	}

	
  //light2.position.set( Math.sin(time*0.2)*14, 0, 25 );
  plane.geometry.computeFaceNormals();
  //plane.geometry.computeVertexNormals();
  plane.geometry.verticesNeedUpdate=true;

  controls.update();

  renderer.render( scene, camera );	  
  requestAnimationFrame( render );
};

render();
