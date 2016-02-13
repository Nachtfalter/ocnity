/*-- Pilotrace --*/

protected func InitializePlayer() {
	//SetFoW (false);
}
	
protected func Initialize() {
	// Create parkour goal & checkpoints.
	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	var checkpoint_padding = 50;
	goal->SetStartpoint(checkpoint_padding, 140);
	goal->SetFinishpoint(LandscapeWidth() - checkpoint_padding, 145);
	
	var checkpoint_count = 3;
	var checkpoint_distance = (LandscapeWidth() - 2 * checkpoint_padding) / (checkpoint_count + 1);
	for (var i = 1; i <= checkpoint_count; i++) {
		goal->AddCheckpoint(
			checkpoint_padding + i * checkpoint_distance,
			RandomX(checkpoint_padding, LandscapeHeight() - checkpoint_padding),
			mode);
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
