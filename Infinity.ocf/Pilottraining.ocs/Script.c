/*-- Pilotrace --*/

static const CHECKPOINT_COUNT = 3;
static const CHECKPOINT_PADDING = 50;

protected func InitializePlayer() {
	//SetFoW (false);
}

// Tries to find a suitable position in the air.
private func FindCheckpointPosition(int x) {
	var i = 100;
	while (i--) {
		var y = RandomX(CHECKPOINT_PADDING, LandscapeHeight() - CHECKPOINT_PADDING);
		if (!GBackSemiSolid(x, y)) break;
	}
	return {x = x, y = y};
}
	
protected func Initialize() {
	// Create parkour goal & checkpoints.
	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	goal->SetStartpoint(CHECKPOINT_PADDING, 140);
	goal->SetFinishpoint(LandscapeWidth() - CHECKPOINT_PADDING, 145);
	
	var checkpoint_distance = (LandscapeWidth() - 2 * CHECKPOINT_PADDING) / (CHECKPOINT_COUNT + 1);
	for (var i = 1; i <= CHECKPOINT_COUNT; i++) {
		var p = FindCheckpointPosition(CHECKPOINT_PADDING + i * checkpoint_distance);
		goal->AddCheckpoint(p.x, p.y, mode);
		// Add a permanent draft for each checkpoint.
		var draft = CreateObject(Draft, p.x, p.y + 50);
		draft->SetPermanent();
	}

	//  Place Drafts
	for (var count = 0; count < 20; count++) {
		var draft = CreateObject(Draft);
		// Most drafts look up.
		var r = RandomX(-90, 90);
		if (!Random(3)) r += 180;
		draft->SetR(r);
	}

	PlaceObjects(Loam, 25, "Earth");
	PlaceGrass(1000);
}

// Gamecall from parkour goal, on respawning.
protected func OnPlayerRespawn(int plr, object cp) {
	var clonk = GetCrew(plr);
	clonk->CreateContents(Shovel);
}
