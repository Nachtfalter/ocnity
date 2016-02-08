/* Mathematical helper functions */

/*	Function: Rotate
	Rotates the point p = {x, y} around origin by angle degrees.
	
	Returns a proplist {x, y} with the resulting point. */
global func Rotate(int angle, proplist p, proplist origin, int prec) {
	if (origin == nil) origin = {x = 0, y = 0};
	var x = p.x - origin.x, y = p.y - origin.y;
	var xr = Cos(angle, x, prec) - Sin(angle, y, prec),
	    yr = Sin(angle, x, prec) + Cos(angle, y, prec);
	return {
		x = xr + origin.x,
		y = yr + origin.y
	};
}
