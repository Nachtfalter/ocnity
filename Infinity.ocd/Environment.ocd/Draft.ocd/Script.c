/*	Script: Drafts
	Drafts accelerate Clonks in the air, although those using the wing suit get a much greater boost.
	By riding the drafts, gliding Clonks can reach higher or more distant places which are normally inaccessible.

	Drafts have limited bounds, inside which they accelerate Clonks in the draft's direction.
	The number of drafts is determined by the number of activated drafts rules: For each rule, one draft is placed.

	A draft's position is chosen randomly, where it will stay for a configurable duration.
	You can specify a minimum and maximum value, the actual duration a draft stays is then randomised within these bounds.
	*Note:* This behaviour can be overridden by implementing a function called "IsDraftPermanent" that returns true.
	This function may be defined in the scenario script or any rule, goal or environment object.
	If such a function exists, drafts will only set their position once and then stay. */


/*	Variables: Draft size and appearance
	draftWidth			- Width of the draft's bounds, without rotation applied.
	draftHeight			- Height of the draft's bounds, without rotation applied.
	draftDistance		- Distance in which Clonks are searched for, automatically calculated from draftWidth and draftHeight. */
local draftWidth, draftHeight, draftDistance;
/*	Variables: Random placement
	These variables determine the duration that a draft keeps its position, before repositioning itself randomly.

	permanentDraft      - _true_ if this draft is permanent.
	minDraftDuration	- Minimum duration.
	maxDraftDuration	- Maximum duration. */
local permanentDraft, minDraftDuration, maxDraftDuration;
/*	 Variables: Clonk acceleration
	maxGliderSpeedUpwards	- Maximum speed a glider is accelerated to.
	gliderAcceleration		- Acceleration rate for gliders. Unit is 1/10 pixels per frame. */
local maxGliderSpeedUpwards, gliderAcceleration;

local active;

protected func Initialize() {
	SetSize(50, 150);
	minDraftDuration = 1050;
	maxDraftDuration = 2100;
	maxGliderSpeedUpwards = 60;
	gliderAcceleration = 5;
	// allow for setting it permanent
	ScheduleCall(this, "SetRandomPosition", 1);
	Enable();
}

/*	Function: SetSize
	Sets the draft's bounds inside which it accelerates Clonks.
	The bounds are specified for unrotated drafts.
	If a draft is rotated, the searching area is rotated automatically as well.

	Parameters:
	width 	- Bounds width.
	height	- Bounds height. */
public func SetSize(int width, int height) {
	draftWidth = width;
	draftHeight = height;
	draftDistance = Sqrt(draftWidth ** 2 + draftHeight ** 2) / 2;
}

/*  Function: SetPermanent
	Makes this draft permanent. */
public func SetPermanent() {
	permanentDraft = true;
}

/*  Function: Enable
	Enables this draft. */
public func Enable() {
	if(!GetEffect("Draft", this)) {
		AddEffect("Draft", this, 1, 10, this);
		AddEffect("DraftParticle", this, 1, 1, this);
		return true;
	}
}

/*  Function: Disable
	Disables this draft.

	A disabled draft will continue to exist and will keep its position. Repositioning will still occur.

	A disabled draft can be re-enabled with <Enable>. */
public func Disable() {
	RemoveEffect("Draft", this);
	RemoveEffect("DraftParticle", this);
	return true;
}

protected func FxDraftParticleStart(object target, proplist effect, int temporary) {
	if (temporary) return FX_OK;
	effect.particles = {
		Prototype = Particles_Air(),
		Size = PV_Random(2, 5)
	};
}

protected func FxDraftParticleTimer(object target, proplist effect, int effectTime) {
	var draftDirection = GetR();
	var speed = RandomX(-draftHeight / 6, -draftHeight / 4);

	// Create air particles.
	var particle_count = draftHeight * draftWidth / 500;
	for (var count = 0; count < particle_count; count++) {
		var x = Random(draftWidth) - draftWidth / 2;
		var y = RandomX(0, Max(3 * draftHeight / 4, draftHeight - speed / 2));
		var xdir = -Sin(draftDirection, speed);
		var ydir =  Cos(draftDirection, speed);
		var point_angle = Angle(0, 0, x, y);
		var point_d = Distance(x, y);
		x = -Sin(draftDirection + point_angle, point_d);
		y = Cos(draftDirection + point_angle, point_d);	
		CreateParticle("Air", x - xdir / 4, y - ydir / 4, xdir, ydir, PV_Random(14, 26), effect.particles, 1);
	}
}

protected func FxDraftTimer(object target, proplist effect, int effectTime) {
	var draftDirection = GetR();
	var p = {x = 0, y = -draftHeight / 2};
	var rp = Rotate(draftDirection, p);
	var gliders = FindObjects(Find_NoContainer(), Find_Distance(draftDistance, rp.x, rp.y), Find_Category(C4D_Living));
	if(GetLength(gliders)) {
		if(!active) {
			active = true;
			effect.Interval = 1;
		}
		for( var glider in gliders ) {
			var gp = {x = glider->GetX() - GetX(), y = glider->GetY() - GetY()};
			var rgp = Rotate(-draftDirection, gp);
			if(Inside(rgp.x, -draftWidth / 2, draftWidth / 2) && Inside(rgp.y, -draftHeight, 0)) {
				var xDir = glider->GetXDir(100), yDir = glider->GetYDir(100);
				var xAcc = Sin(draftDirection, gliderAcceleration * 10), yAcc = -Cos(draftDirection, gliderAcceleration * 10);
				// Not actually gliding? Less acceleration!
				if(!glider->~IsGliding()) {
					xAcc /= 3;
					yAcc /= 3;
				}
				glider->SetXDir(xDir + xAcc, 100);
				//Message("X: %d, Y: %d", this, xAcc, yAcc);
				if(yDir > -maxGliderSpeedUpwards * 10) {
					glider->SetYDir(yDir + yAcc, 100);
				}
			}
		}
	} else if(active) {
		active = false;
		effect.Interval = 10;
	}
}

protected func SetRandomPosition() {
	if(permanentDraft)
		return;
	var x = Random(LandscapeWidth());
	var y = Random(LandscapeHeight());
	var otherDraft = FindObject( Find_And( Find_ID(GetID()), Find_InRect(AbsX(x) - draftWidth / 2, AbsY(y) - draftHeight, draftWidth, draftHeight) ) );
	if(otherDraft != nil) {
		// Anderes Aufwind-Objekt schon an der Stelle: Nächstes Frame neue Position suchen.
		ScheduleCall(this, "SetRandomPosition", 1);
	} else {
		if( !GameCallEx("IsDraftPermanent") ) {
			// Kein anderes Aufwind-Objekt: Neue Position erst in einer Weile suchen.
			ScheduleCall(this, "SetRandomPosition", RandomX(minDraftDuration, maxDraftDuration));
		}
		SetPosition(x, y);
	}
}
