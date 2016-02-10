/* Wing Suit! */

#appendto Clonk

protected func ObjectControlMovement(int plr, int ctrl, int strength, bool release, bool repeat) {
	if(ctrl == CON_Down && !release && (IsJumping() || GetAction() == "Dive")) {
		if(IsGliding()) {
			RemoveEffect("WingSuit", this);
		} else {
			AddEffect("WingSuit", this, 150, 1, this);
		}
		return true;
	}
	if(!release && IsGliding()) {
		if(ctrl == CON_Left) {
			SetDir(DIR_Left);
		} else if(ctrl == CON_Right) {
			SetDir(DIR_Right);
		}
	}
	return _inherited(plr, ctrl, strength, release, repeat);
}

public func IsGliding() {
	return GetEffect("WingSuit", this) != nil;
}

protected func FxWingSuitStart(object target, proplist effect, int temporary) {
	if(!temporary) {
		// TODO: Sound
		/* Sound("SailDown", false, 50); */
		if(!IsJumping())
			SetAction("Jump");

		effect.maxGlideSpeedY = 30;
		effect.glideSpeedXFactor = 2;
		effect.particles = {
			Prototype = Particles_Air(),
			Size = PV_Random(1, 3)
		};
	}
}

protected func FxWingSuitTimer(object target, proplist effect, int effectTime) {
	if(IsJumping()) {
		// Maximale Sinkgeschwindigkeit
		if( GetYDir() > effect.maxGlideSpeedY ) {
			SetYDir( GetYDir() - 2 );
		}
		// Horizontale Geschwindigkeit berechnen und anpassen
		var targetXDir = Max( Abs(GetYDir()), 5 ) * (-effect.glideSpeedXFactor + 2 * effect.glideSpeedXFactor * GetDir());
		var currentXDir = GetXDir();
		if(currentXDir < targetXDir) {
			SetXDir( GetXDir() + 1 );
		} else if(currentXDir > targetXDir) {
			SetXDir( GetXDir() - 1 );
		}
		// Rotiert die Jump-Animation, sodass es aussieht, als würde der Clonk einen Wingsuit tragen.
		var s = 1000 - 2000 * GetDir();
		SetObjDrawTransform(0, s, 0, -s, 0, 0);
		if(!Random(9)) {
			CreateParticle("Air", RandomX(-7, 7), RandomX(-3, 3), GetXDir() / 2, GetYDir() / 2, 40, effect.particles);
		}
		return FX_OK;
	} else {
		return FX_Execute_Kill;
	}
}

protected func FxWingSuitStop(object target, proplist effect, int reason, bool temporary) {
	if(!temporary) {
		// TODO: Sound
		/* Sound("SailUp", false, 50); */
		SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, 0);
	}
}

/* Prevents the Clonk from tumbling after hitting something while gliding. */
protected func Hit(int xdir, int ydir) {
	if(IsGliding() && GetAction() == "Tumble") {
		SetXDir(BoundBy(xdir, -10, 10));
		SetYDir(BoundBy(ydir, -10, 10));
		SetAction("Jump");
	}
	return _inherited(xdir, ydir, ...);
}

