/*-- Pilotrace --*/

protected func Initialize()
{
	// Create parkour goal & checkpoints.
	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	goal->SetStartpoint(90, 345);
	goal->AddCheckpoint(630, 215, mode);
	goal->AddCheckpoint(211, 105, mode);
	goal->SetFinishpoint(80, 345);
	
	// weil Baum
	for(var i = 1; i <= 3; ++i)
		PlaceVegetation(Tree_Coniferous3, 0, 0, 1000, 1000, -1);
}

// Gamecall from parkour goal, on respawning.
protected func OnPlayerRespawn(int plr, object cp)
{
	var clonk = GetCrew(plr);
	clonk->CreateContents(Boompack);
}

global func FxRespawnBoomTimer(object target, effect, int time)
{
	target->CreateContents(Boompack);
	return -1;
}
