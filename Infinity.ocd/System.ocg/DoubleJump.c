#appendto Clonk

local doubleJumpPossible;
local maxDoubleJumpStartSpeed, doubleJumpAcceleration;

protected func Initialize() {
	maxDoubleJumpStartSpeed = 50;
	doubleJumpAcceleration = 34;
	return _inherited(...);
}

protected func ObjectControlMovement(int plr, int ctrl, int strength, bool release, bool repeat) {
	if(ctrl == CON_Jump && !release && (IsJumping() || GetAction() == "Dive") && doubleJumpPossible) {
		doubleJumpPossible = false;
		if( GetDir() == DIR_Left ) {
			SetXDir( -1 * Abs( GetXDir() ) );
		} else {
			SetXDir( Abs( GetXDir() ) );
		}
		// To make double jumping useful when flying upwards, add more speed
		if(GetYDir() < 0) {
			SetYDir(-Sqrt(GetYDir() ** 2 + doubleJumpAcceleration ** 2));
		} else {
			SetYDir(-doubleJumpAcceleration);
		}
		JumpEffect("Up");
		return true;
	}
	return _inherited(plr, ctrl, strength, release, repeat);
}

protected func StartJump() {
	doubleJumpPossible = true;
	AddEffect("CheckMaxDoubleJumpSpeed", this, 10, 10, this);
	return _inherited(...);
}

protected func FxCheckMaxDoubleJumpSpeedTimer() {
	if(IsJumping()) {
		// Visual clue that double jump is not available any more: Tumble if falling speed is too high.
		if(GetYDir() > maxDoubleJumpStartSpeed && !(this->~IsGliding())) {
			SetAction("Tumble");
		} else {
			return FX_OK;
		}
	}
	return FX_Execute_Kill;
}

// Stolen from Knüppeln.c4d
private func JumpEffect(string dir) {
	var from, to;

	if (dir == "Up")
	{
		from = 50; 
		to = 130;
	}
	if (dir == "Left")
	{
		from = -30; 
		to = 50;
	}
	if (dir == "Right")
	{
		from = 130; 
		to = 210;
	}
	if (dir == "Down")
	{
		from = 230; 
		to = 310;
	}

	Sound("Fire::Inflame", false, 30, nil, nil, nil, 200);
	
	var trailparticles =
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 6), 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-20, 20, 10)),
		ForceY = PV_Linear(0, PV_Random(-20, 20, 10)),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	
	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = Cos(i, r);
		var y = Sin(i, r);
		CreateParticle("StarFlash", x, y, Cos(i, r), Sin(i, r), 60, trailparticles, 10);
	}
}
