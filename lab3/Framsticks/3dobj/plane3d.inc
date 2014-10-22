//functions supporting drawing of an additional plane for whitevoid.style

function sqr(x) {
   return x*x;
}

// vector[3] * scalar
function mulvs(v,s) {
    return [v[0]*s, v[1]*s, v[2]*s];
}

// vector[3] + vector[3]
function addvv(v1,v2) {
    return [v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]];
}

function subvv(v1,v2) {
    return [v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]];
}

function mulvv(v1,v2) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

function normalize(v) {
  var a = Math.sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
  return mulvs(v,1.0/a);
}

// plane: r, v: vector[3]
// point: t: vector[3]
function signedDistance(r, v, t) {
    return mulvv(v, subvv(t, r));
}

// plane: r, v: vector[3]
// point: t: vector[3]
function projectPointOnPlane(r, v, t) {
    var d = signedDistance(r, v, t);
    return subvv(t, mulvs(v,d));
}

// Finds a perpendicular vector for a given vector[3] v. The result is quite arbitrary
function getPerpendicularV0(v) {
  var v0;
  if (v[2] != 0.0) {
      v0 = [0.0, 1.0, -v[1]/v[2]];
  } else if (v[1] != 0.0) {
      v0 = [1.0, -v[0]/v[1], 0.0];
  } else {
      v0 = [-v[2]/v[0], 0.0, 1.0];
  }
  return normalize(v0);
}

// Finds a perpendicular vector for a given perpendicular vectors[3] v,v0
function getPerpendicularV1(v, v0) 
{
  var v1 = [0.0, 0.0, 0.0];
  var Wz =  v0[0]*v[1] - v[0]*v0[1];
  var Wx = -v0[2]*v[1] + v[2]*v0[1];
  var Wy =  v0[2]*v[0] - v[2]*v0[0];

  if (Wz != 0.0) {
      v1.set(2, Math.sqrt(1.0/(Wx*Wx + Wy*Wy + Wz*Wz)));
      v1.set(0, v1[2]*(Wx/Wz));
      v1.set(1, v1[2]*(Wy/Wz));
  } else if (Wx != 0.0) {
      v1.set(0, Math.sqrt(1.0/(Wx*Wx + Wy*Wy + Wz*Wz)));
      v1.set(1, v1[0]*(Wy/Wx));
      v1.set(2, v1[0]*(Wz/Wx));
  } else {
      v1.set(1, Math.sqrt(1.0/(Wx*Wx + Wy*Wy + Wz*Wz)));
      v1.set(2, v1[1]*(Wz/Wy));
      v1.set(0, v1[1]*(Wx/Wy));
  }

  return normalize(v1);
}

// prints vector[3] v
function printv(desc,v) {
    Simulator.print(desc + ": " + v[0] + " " + v[1] + " " + v[2]);
}

// v: vector v; d: float
function getPlanePoint(v, d) {
  if (v[0] != 0.0) {
    return [-d/v[0], 0.0, 0.0];
  } else if (v[1] != 0.0) {
    return [0.0, -d/v[1], 0.0];
  } else {
    return [0.0, 0.0, -d/v[2]];
  }		
}

// is point inside bbox (min, max)?
// pnt,min,max: vector[3]
function isInside(pnt, min, max) {
  var k;
  for (k=0; k<3; ++k) {
    if (pnt[k] < min[k] || (pnt[k] > max[k]))
      return 0;
  }
  return 1;
}

// adjust quad to bbox using c as a central point and perpendicular vectors v0,v1
// min, max - bbox (Vectors)
// quad: Vector[4][3]
function crop(c, v0, v1, min, max) {
  var i;
  var v = [v0, v1, mulvs(v0, -1.0), mulvs(v1, -1.0)];

  // For every direction v[0-4]
  var r = [c,c,c,c];      
  for (i=0; i<4; ++i) {
    var coeff = 1.0;
    while (coeff > 0.01) {
      var t = addvv(r[i], mulvs(v[i], coeff));
      if (isInside(t, min, max))
        r.set(i, t);
      else
        coeff /= 2.0;
    }
  }

  var quad = [
    subvv(addvv(r[0],r[1]),c),
    subvv(addvv(r[1],r[2]),c),
    subvv(addvv(r[2],r[3]),c),
    subvv(addvv(r[3],r[0]),c)
  ];
//  printv("c: ", c);
//  printv("v0: ", v0);
//  printv("v1: ", v1);
//  printv("min: ", min);
//  printv("max: ", max);
//  for (i=0; i<4; ++i)
//      printv(""+i+": ", quad[i]);
  return quad;
}

// plane: v = [a,b,c], d
// bbox: min,max: vector[3]
function createPlaneVertices(v, d, min, max) {

  v = normalize(v);
  var r = getPlanePoint(v, d);
  var wc = mulvs(addvv(max,min), 0.5);
  var c = projectPointOnPlane(r, v, wc);
  var v0 = getPerpendicularV0(v);
  var v1 = getPerpendicularV1(v, v0);

  var quad = crop(wc, v0, v1, min, max);

  var i;
  for (i=0; i<4; ++i)
      VertexBuilder.addVertex(quad[i]);
}

