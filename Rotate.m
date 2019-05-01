phi = -0.076613;
theta = 0.048582;
psi = 0.784805;

sqrt(54^2 + (-72)^2 + 954^2)
x = 54;
y = -72;
z = 954;

  phi =  atan(y / sqrt( z^2 + x^2)) ;
  theta = atan( -x / sqrt(y^2 + z^2))  ;
  psi = atan(z / sqrt(y^2+x^2) );
  
x = 90;
y = 918;
z = -180;
  
  
 % North-East-Down reference frame (see Figure)
 % Roll(phi) Pitch(theta) Yaw(psi), angles in radians
 % derotation matrix coefficients (rotation matrix is transposed)
 m11= cos(theta)*cos(psi);
 m21= cos(theta)*sin(psi);
 m31=-sin(theta);
 m12=sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi);
 m22=sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi);
 m32=sin(phi)*cos(theta);
 m13=cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi);
 m23=cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi);
 m33=cos(phi)*cos(theta);
 % derotate vector (product matrix by vector)
 xr = m11*x + m12*y + m13*z;
 yr = m21*x + m22*y + m23*z;
 zr = m31*x + m32*y + m33*z;
 
   phi2 =  atan(yr / sqrt( zr^2 + xr^2)) * 57,3;
  theta2 = atan( -xr / sqrt(yr^2 + zr^2)) *57,3 ;

