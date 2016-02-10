/*-- Pilotrace --*/

protected func InitializePlayer() {
    //SetFoW (false);
 }
 
protected func Initialize()
{
	// Create parkour goal & checkpoints.
	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	goal->SetStartpoint(50, 140);
	goal->SetFinishpoint(3145, 145);
	

	for (var count = 0; count < 20; count++)
	{
		var draft = CreateObject(Draft);
		draft->SetR(Random(360));
	}

	PlaceObjects(Loam, 25, "Earth");
	PlaceGrass(1000);
}

/*
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
}*/
