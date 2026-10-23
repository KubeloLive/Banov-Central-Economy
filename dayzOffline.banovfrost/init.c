void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();
	
	GetCEApi().ExportProxyData(vector.Zero, 0);
	//GetCEApi().ExportClusterData();
	

	// BANOVFROST: lock map to December for a true winter solar cycle.
	int year, month, day, hour, minute;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);
	if (month != 12 || day != 15)
		GetGame().GetWorld().SetDate(year, 12, 15, hour, minute);

}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.45, 0.65 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
        // BANOVFROST: fixed starter clothing. All other spawn equipment is unchanged.
        EntityAI oldBody = player.FindAttachmentBySlotName("Body");
        if (oldBody)
            GetGame().ObjectDelete(oldBody);
        player.GetInventory().CreateAttachment("Hoodie_Red");

        EntityAI oldLegs = player.FindAttachmentBySlotName("Legs");
        if (oldLegs)
            GetGame().ObjectDelete(oldLegs);
        player.GetInventory().CreateAttachment("MensShalwarPants_Black");

		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "BandageDressing" );
			player.SetQuickBarEntityShortcut(itemEnt, 2);

            // Frost spawn: small survival aid for sub-zero weather.
            itemClothing.GetInventory().CreateInInventory("Heatpack");

			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt( 0, 4 );
			itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
			player.SetQuickBarEntityShortcut(itemEnt, 1);
			SetRandomHealth( itemEnt );
		}
		
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
			SetRandomHealth( itemClothing );
		
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}