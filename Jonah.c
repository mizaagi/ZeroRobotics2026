//When the plot was last watered
double waterTimes[6];
//Plant time for the certain plot
double PT[6];
//Holds the plant's grow time
double PlantTime[6];
//Contains the amount of times watered on a plot used to determine whether to plant, water, or harvest
int charge[6];
//The current value of crops in correct id order
double cropValue[6];
// variable to check which plot of the cycle in loop I am checking.
int count;
// How many times I have visited Astronaut
int astro;
// stores the crop id of the bonus crop, set to 0 if no bonus crop
int bonus;

bool needastro;

void init(){
    // initializers to appropriate values.
    bonus = 0;
    astro = 0;
	waterTimes[0],waterTimes[1],waterTimes[2],waterTimes[3],waterTimes[4],waterTimes[5] = 0,0,0,0,0,0;
	PT[0],PT[1],PT[2],PT[3],PT[4],PT[5] = 0,0,0,0,0,0;
	PlantTime[0], PlantTime[1], PlantTime[2], PlantTime[3], PlantTime[4], PlantTime[5] = 6.0, 8.0, 5.0, 12.0, 3.0, 9.0;
	charge[0], charge[1], charge[2], charge[3], charge[4], charge[5] = 0,0,0,0,0,0;
	cropValue[0] = 6.0;
	cropValue[1] = 7.0;
	cropValue[2] = 5.0;
	cropValue[3] = 11.0;
	cropValue[4] = 3.0;
	cropValue[5] = 8.0; 
	count = 0;
	needastro = true;
}

void wCrop(int plotNum, int cropNum) {
    //moves to plot
    game.MovePlot(plotNum);
    //plants the crop given in the 
    game.PlantCrop(plotNum, cropNum);
    waterTimes[plotNum-1] = game.GetTime();
    charge[plotNum-1] = 1;
    PT[plotNum-1] = cropNum == 1 ? 6.0 : cropNum == 2 ? 8.0 : cropNum == 3 ? 5.0 : cropNum == 4 ? 12.0 : cropNum == 5 ? 3.0 : 9.0;
    game.WaterCrop(plotNum);
    int i = cropValue[cropNum-1];
    cropValue[cropNum-1] = cropValue[cropNum-1] * 0.75;
    if (cropNum == bonus) bonus = 0;
}
void WaterCrop(int plotNum) {
    game.MovePlot(plotNum);
    game.WaterCrop(plotNum);
    waterTimes[plotNum-1] = game.GetTime();
    charge[plotNum-1] = 2;
    if (240-game.GetTime() > 20 && game.GetWaterUnits() == 0) {
        game.MoveWatering(); 
        game.FillWateringCan();
    }
}
void HarvestCrop(int plotNum) {
    int astronumprior = game.GetCurrentBonusIndex();
    int hi = game.GetHarvestScore();
    game.MovePlot(plotNum);
    game.HarvestCrop(plotNum);
    int astronumafter = game.GetCurrentBonusIndex();
    int ne = game.GetHarvestScore();
    if (hi != ne) {
        if (astronumafter > astronumprior && astronumafter != 3) {
            astro+=1;
            game.MoveAstronaut();
            bonus = game.GetBonusCrop();
            int placeholder = game.GetCurrentBonusIndex();
            DEBUG(("place %d", placeholder));
            if (bonus == 0) needastro = true;
            else needastro = false;
            
        }
        charge[plotNum-1] = 0;
        waterTimes[plotNum-1] = 0;
        PT[plotNum-1] = 0;
        // helper(plotNum);
    }
    // DEBUG(("%d", charge[plotNum-1]));
}

void helper(int pn) {
    if (needastro && game.GetPlotsExplored() >= 4 && bonus == 0) {
        game.MoveAstronaut();
        bonus = game.GetBonusCrop();
        if (bonus == 0) needastro = true;
        else needastro = false;
    }
    int bestcrop = 1;
    for (int i = 0; i < 6; i++) {
        bestcrop = cropValue[i] > cropValue[bestcrop-1] ? i+1 : bestcrop;
    }
    if (bonus != 0) bestcrop = bonus;
    float Time = game.GetTime();
    if (240-Time < 16) {
        if (Time-waterTimes[pn-1] > PT[pn-1] && charge[pn-1] == 2) {
            HarvestCrop(pn);
            if (Time < 230) {
                if (Time-waterTimes[pn-1] > 2.2*PlantTime[bestcrop-1]) wCrop(pn, bestcrop);
                else wCrop(pn, 5);
            }
            if (game.GetWaterUnits() == 0 && Time < 232) {
                game.MoveWatering(); 
                game.FillWateringCan();
            }
            
        }
         else if (charge[pn-1] == 1 && 240-Time > PT[pn-1] && Time-waterTimes[pn-1] > PT[pn-1] && Time < 230) {
            WaterCrop(pn);
            if (game.GetWaterUnits() == 0 && Time < 232) {
                game.MoveWatering(); 
                game.FillWateringCan();
            }
            bool someelsebetter = false;
            for (int i = 0; i < 6; i++) {
                if (PT[i] + waterTimes[i] < PT[pn-1]+waterTimes[pn-1] && PT[i] != 0) someelsebetter = true;
            }
            if (!someelsebetter) {
                game.SetWait(PT[pn-1]);
                HarvestCrop(pn);
            }
        }
    } else {
        if (game.GetTime() == 0) {wCrop(pn, bestcrop);charge[pn-1] = 1;}
        DEBUG(("TIME: %.2f", Time));
        DEBUG(("WT: %.2f", waterTimes[pn-1]));
        DEBUG(("PT: %.2f", PT[pn-1]));
        if (Time-waterTimes[pn-1] > PT[pn-1]-2) {
            DEBUG(("Plot: %d", pn));
            if (charge[pn-1] == 2) {
                HarvestCrop(pn);
                
                helper(pn);
            }
            else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
            else {wCrop(pn, bestcrop);charge[pn-1] = 1;}
        }
    }
}

void helper(int pn, int cropNum) {
    if (game.GetTime()-waterTimes[pn-1] > PT[pn-1]) {
        if (charge[pn-1] == 2) {HarvestCrop(pn); helper(pn);}
        else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
        else {wCrop(pn, cropNum); charge[pn-1] = 1;}
    }
}

void dohelp(int pn) {
    if (game.GetTime()-waterTimes[pn-1] > PT[pn-1] && charge[pn-1] == 2) {
        HarvestCrop(pn);
    }
    else if (charge[pn-1] == 1 && game.GetTime()-waterTimes[pn-1] > PT[pn-1]) {
        WaterCrop(pn);
    }
}

void wCropFinal40(int plotNum, int cropNum) {
	// Function to move to a plot, plant a specific crop, and set time data for the crop
    game.MovePlot(plotNum);
    game.PlantCrop(plotNum, cropNum);
    waterTimes[plotNum-1] = game.GetTime();
    charge[plotNum-1] = 1;
	// Determines what the crop's plant time is and sets the PT times based on the crop number
    PT[plotNum-1] = cropNum == 1 ? 6.0 : cropNum == 2 ? 3.0 : cropNum == 3 ? 5.0 : cropNum == 4 ? 12.0 : cropNum == 5 ? 3.0 : 9.0;
    game.WaterCrop(plotNum);
	// Crop value depreciation
    cropValue[cropNum-1] = cropValue[cropNum-1] * 0.75;
	// If the crop being planted is the bonus crop, set the bonus crop variable to zero to show that a new bonus crop can be generated
    if (cropNum == bonus) bonus = 0;
}

// FINAL 40 CODE IS BELOW
void helperFinal40(int pn) {
	// helper determines if robot is able to do something with the plot
    int bestcrop = 1;
    for (int i = 0; i < 3; i++) {
        bestcrop = cropValue[i] > cropValue[bestcrop-1] ? i+1 : bestcrop;
    }
    // DEBUG(("%.2f", cropValue[bestcrop-1]));
    if (game.GetTime()-waterTimes[pn-1] > PT[pn-1]-2) {
        // DEBUG(("%d", charge[pn-1]));
        if (charge[pn-1] == 2) {
            HarvestCrop(pn);
        }
        else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
        else {wCropFinal40(pn, bestcrop);charge[pn-1] = 1;}
    }
}

void final40() {
    // Assume that bonus crops have been obtained
    // id 5, 3, 1
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    DEBUG(("hidhfishdofsaijpfnwojfniuadbvijsdanovjabsihprfuiognrjv"));
    cropValue[1] = cropValue[4];
    while (game.GetTime() < 239) { // Runnable because at this point we are just running code until time runs out
        int f = 0; // Variable to simulate for loop within while loop; traverse through list of plots;
        // After watering, i will reset to zero so the loop can continue if possible
        int plotTravel[4] = {2, 6, 3, 1}; // Desired path
        if (game.GetWaterUnits() > 0) {
            helperFinal40(plotTravel[f]);
            f++;
            if (f == 1 ){
                dohelp(5);
            } else if (f == 2) {
                dohelp(4);
            }
            if (f == 4) f = 0;
        } else {
            game.MoveWatering();
            game.FillWateringCan();
        }
    }
}


void loop() {
    if (count == 0) {
        cropValue[0] = 6.0;
	cropValue[1] = 7.0;
	cropValue[2] = 5.0;
	cropValue[3] = 11.0;
	cropValue[4] = 3.0;
	cropValue[5] = 8.0; 
        DEBUG(("%.2f", cropValue[0]));
    // wCrop(1,2);
    helper(1);
// 	wCrop(3,4);
	helper(3);
// 	wCrop(6,6);
    helper(6);
// 	wCrop(5,1);
	helper(5);
	game.MoveAstronaut();
    bonus = game.GetBonusCrop();
// 	wCrop(4,bonus);
	helper(4);
// 	WaterCrop(3);
	helper(3);
	
	game.MoveWatering(); 
	game.FillWateringCan();
	game.GetWaterUnits();
	count = 1;
  } else if (astro < 2) {
  if (count == 1) {
      helper(1);
      count = 3;
  }
  if (count == 2) {
      helper(2);
      
      if (game.GetTime() < 220) {
        game.MoveWatering(); 
        game.FillWateringCan();
      }
      count = 1;
  }
  if (count == 3) {
      helper(3);
      count = 4;
  }
  if (count == 4) {
      helper(4);
        helper(5);
        count = 6;
      
  }
  if (count == 6) {
      helper(6);
      count = 2;
  }
  } else {
    if (count == 1) {
        helper(1);
        count = 3;
    }
  if (count == 2) {
      helper(2);
      DEBUG(("BROTHER"));
      
      if (game.GetTime() < 220) {
        game.MoveWatering(); 
        game.FillWateringCan();
      }
      count = 1;
  }
  if (count == 3) {
      helper(3);
      dohelp(4);
      dohelp(5);
      count = 6;
  }  
  if (count == 6) {
      helper(6);
      count = 2;
  }
//   }
    // final40();
  }
  if (game.GetTime() < 239) loop();
  else {
      game.GetDistanceTraveled(); game.GetPlotsExplored(); game.GetCurrentBonusIndex();
      for (int i = 0; i < 6; i++) DEBUG(("%.2f", cropValue[i]));
  }
	
}
