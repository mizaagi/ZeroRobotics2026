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
            if (pn == 5) bccheck = true;
        }
        else if (charge[pn-1] == 1) {WaterCrop(pn);charge[pn-1] = 2;}
        else {wCrop(pn, bestcrop);charge[pn-1] = 1;}
    }
}

void final40() {
    // Assume that bonus crops have been obtained
    while (true) { // Runnable because at this point we are just running code until time runs out
        i = 0; // Variable to simulate for loop within while loop; traverse through list of plots;
        // After watering, i will reset to zero so the loop can continue if possible
        int[] plotTravel = {6, 3, 1}; // Desired path
        while (game.getWaterUnits() > 0) {
            helperFinal40(i);
            i++;
            if (i == 3) i = 0;
        }
        game.MoveWatering();
        game.FillWateringCan();
    }
}
