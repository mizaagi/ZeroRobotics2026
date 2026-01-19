//When the plot was last watered
double waterTimes[6];
//Plant time for the certain plot
double PT[6];

int charge[6];
double cropValue[6];
int count;
int astro;
bool bccheck;
int bonus;
double p[6];

void init(){
    bonus = 0;
    bccheck = true;
    astro = 0;
	waterTimes[0],waterTimes[1],waterTimes[2],waterTimes[3],waterTimes[4],waterTimes[5] = 0,0,0,0,0,0;
	PT[0],PT[1],PT[2],PT[3],PT[4],PT[5] = 0,0,0,0,0,0;
	p[0], p[1], p[2], p[3], p[4], p[5] = 6.0, 8.0, 5.0, 12.0, 3.0, 9.0;
	charge[0], charge[1], charge[2], charge[3], charge[4], charge[5] = 0,0,0,0,0,0;
	cropValue[0] = 6.0;
	cropValue[1] = 7.0;
	cropValue[2] = 5.0;
	cropValue[3] = 11.0;
	cropValue[4] = 3.0;
	cropValue[5] = 8.0; 
// 	DEBUG(("%.2f", cropValue[5]));
	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	count = 0;
}

void wCrop(int plotNum, int cropNum) {
    game.MovePlot(plotNum);
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
    int hi = game.GetHarvestScore();
    game.MovePlot(plotNum);
    game.HarvestCrop(plotNum);
    int ne = game.GetHarvestScore();
    if (hi != ne) {
        if (ne-hi > PT[plotNum-1] && astro < 2) {
            astro++;
            game.MoveAstronaut();
            bonus = game.GetBonusCrop();
        }
        charge[plotNum-1] = 0;
        waterTimes[plotNum-1] = 0;
        PT[plotNum-1] = 0;
    }
    // DEBUG(("%d", charge[plotNum-1]));
}

void helper(int pn) {
    if (240-game.GetTime() < 10) {
        // DEBUG(("hi"));
        if (game.GetTime()-waterTimes[pn-1] > PT[pn-1] && charge[pn-1] == 2) {
            HarvestCrop(pn);
        }
         else if (charge[pn-1] == 1 && 240-game.GetTime() > PT[pn-1]) {
            WaterCrop(pn);
            game.SetWait(PT[pn-1]);
            game.MoveToVoid(game.GetRobotPositionX(), game.GetRobotPositionY(), game.GetRobotPositionZ()-0.03);
            HarvestCrop(pn);
        }
    } else {
        int bestcrop = 1;
        for (int i = 0; i < 6; i++) {
            bestcrop = cropValue[i] > cropValue[bestcrop-1] ? i+1 : bestcrop;
        }
        // if (bonus != 0) {
        //     bool check = false;
        //     for (int i = 0; i < 6; i++) {
        //         if (PT[i] == p[bonus]) {
        //             check = true;
        //         }
        //     }
        //     if (check) {
        //         bonus = 0;
        //     } else {
        //         bestcrop = bonus;
        //     }
            
        // }
        if (bonus != 0) bestcrop = bonus;
        // DEBUG(("%.2f", cropValue[bestcrop-1]));
        if (game.GetTime()-waterTimes[pn-1] > PT[pn-1]-2) {
            // DEBUG(("%d", charge[pn-1]));
            if (charge[pn-1] == 2) {
                HarvestCrop(pn);
                if (pn == 5) bccheck = true;
            }
            else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
            else {wCrop(pn, bestcrop);charge[pn-1] = 1;}
            // DEBUG(("%d", charge[pn-1]));
        }
    }
}
void helper(int pn, int cropNum) {
    if (game.GetTime()-waterTimes[pn-1] > PT[pn-1]) {
        if (charge[pn-1] == 2) HarvestCrop(pn);
        else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
        else {wCrop(pn, cropNum); charge[pn-1] = 1;}
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
    wCrop(1,2);
	wCrop(3,4);
	wCrop(6,6);
	wCrop(5,1);
	game.MoveAstronaut();
    bonus = game.GetBonusCrop();
    bccheck = false;
	wCrop(4,bonus);
	WaterCrop(3);
	
	game.MoveWatering(); 
	game.FillWateringCan();
	game.GetWaterUnits();
	count = 1;
  }
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
//   bool hi = true;
//   if (game.GetTime() > 220) {
//       for (int i = 0; i < 6; i++) {
//           if (PT[i] != 0) hi = false;
//       }
//       if (hi) {
//         while (game.GetTime() < 239) {
//             game.MovePlot(1);
//             game.MovePlot(4);
//         }
//       }
//   }
  if (game.GetTime() < 239) loop();
  else {game.GetDistanceTraveled(); DEBUG(("%d", astro));}
	
}
