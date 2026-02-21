G4Tubs* hadLayerSolid = new G4Tubs( "HadCaloLayerSolid", //its name
	                                    EDITME:innerradius ,
	                                    EDITME:outerradius ,
	                                    EDITME:length,
	                                    EDITME:start_angle,
	                                    EDITME:end_angle);

G4LogicalVolume* hadLayerLogic = new G4LogicalVolume(EDITME:solid_pointer,
														 EDITME:material,
														 "HadLayerLogic");//its name

G4ThreeVector absorberLayer(0,0,hadCaloFeThickness);
	G4ThreeVector activeLayer(0,0,hadCaloLArThickness);
	G4int layerCopyNum = hadCaloCopyNum;
	for ( int layerIdx = 0 ; layerIdx < hadCaloNumLayers ; ++layerIdx )
	{
		G4ThreeVector position = (layerIdx+1)*absorberLayer + (layerIdx+0.5)*activeLayer;
		position -= G4ThreeVector(0,0,halfHadCaloHalfZ);//Position is w.r.t. center of mother volume: the hadCaloLogic
		new G4PVPlacement(EDITME:rotation,
						  position, //position, do not change this, already done for you
						  EDITME:logical_volume,
						  "HadCaloLayer",//a name
						  EDITME:mother_logical_volume,
						  false, //leave this like this: not used
						  ++layerCopyNum);//The unique number, it will be 1001+layerIndex
        }