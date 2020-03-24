// splash of liquid drop
// biscuit.pov
// http://warp.povusers.org/povtips/scene3.txt
// http://www.f-lohmueller.de/pov_tut/interior/interior_03e.htm 

#declare White=rgb<1,1,1>;
#declare Blue=rgb<0.8,0.8,0.9>;
                                 
#include "colors.inc"                                 
#include "textures.inc"
#include "object.inc"
            
global_settings { assumed_gamma 1.0 }                     
            
camera 
{
  right x*640/360
  location <145,60,97>
  sky <0,0,1>
  look_at <95,60,62>
           
}
    
//camera {location <20,40,28> direction 2*z look_at <0,2,0>}
           

  sky_sphere {
    pigment {
      gradient z
      color_map {
        [0.0 rgb <0.6,0.7,1.0>]
        [0.7 rgb <0.0,0.1,0.8>]
      }
    }
  }           
           
light_source
{
  <306,165,265> White
}

/*light_source
{ <120, 70, 185>, .3
  fade_distance 160 fade_power 2
    area_light x*3, y*3, 12, 12 circular orient adaptive 0
} */
light_source
{ <92, 200, 195>, <.5,.4,.3>
  fade_distance 120 fade_power 2
      area_light x*3, y*3, 12, 12 circular orient adaptive 0
} 

plane
{ z, 0.0
  pigment { rgb <0.7,0.68,0.63> }
}                




intersection{
object{m_}   
cylinder{ <47.5, 47.5, -10><47.5, 47.5,111>45}
                                          
//translate<-1,-1,-8>                                          
translate<0,0,-6>                                          
scale 0.95
  
  

 
 pigment {Orange*0.8 filter 0.2}
      finish {phong 0.7 reflection 0.15}
      normal {bumps 0.05 scale 1} 

/*normal {
   wrinkles 1
   scale 20.6
//   granite
//   scale 4
} */
}  
      
      
      
#declare TeaCupMQ =
union {
   difference {
   cylinder{ <45, 45, 0><45, 45,45>44}
   cylinder{ <45, 45, 1><45, 45,45.1>40}
   }                                              
               
  
   torus {42,2 translate <45,45,-45> rotate x*90 }
   torus {42,2 translate <45,0,-45> rotate x*90 }
   torus {12,3 rotate z*(-10) translate <89,30,25> }
   
   torus {2.1,.8 rotate y*90+x*90+z*(-10) translate <86.5,31.5,15> }
   torus {2.1,.8 rotate y*90+x*90+z*(-10) translate <86.5,31.5,38.2> }
   
//   pigment {Red}
    pigment { 
          image_map {
                png "gfx/palaboslargered3.png"
                map_type 3   
            }  
            rotate x*90-z*40
            scale 63
            translate <10,0,-40>      
          }
                 
   normal {bumps 0.03 scale 3}
   finish {phong 0.8 reflection 0.1}                  
}


object {TeaCupMQ translate<0,0,3.5>}

#declare Podstawka =
union {   
                               
   difference
   {                            
    cylinder {<0,0,0> <0,0,4.0> 70  translate <45,45,0> }
    cylinder {<0,0,2.0> <0,0,5.5> 55  translate <45,45,0> }
   }          
   torus {70,2 rotate x*90 translate <45,45,2> }
   torus {55,2 rotate x*90 translate <45,45,2> }
   
   
                                                                                                      
   pigment {rgb <0,0,0>}                                
   normal {bumps 0.03 scale 3}
   finish {phong 0.2 reflection 0.05}                  
}
                          
object {Podstawka}                          


