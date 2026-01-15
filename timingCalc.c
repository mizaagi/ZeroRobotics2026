void init() {
}


// NOTE: This timing function only gives an ESTIMATE of the time it takes
// to move between two different locations. There is an element of random-
// ness that is included in 
void timing(int plot1, int plot2) {
    // 0 represents Watering, 7 represents Astronaut
    if (plot1 > 0 && plot1 < 7) {
        game.MovePlot(plot1);
    } else if (plot1 == 0) {
        game.MoveWatering();
    } else if (plot1 == 7) {
        game.MoveAstronaut();
    }
    double time1 = game.GetTime();
    
    
    if (plot2 > 0 && plot2 < 7) {
        game.MovePlot(plot2);
    } else if (plot2 == 0) {
        game.MoveWatering();
    } else if (plot2 == 7) {
        game.MoveAstronaut();
    }
    double time2 = game.GetTime();
    
    DEBUG(("%.2f", time2 - time1));
}

void loop() {
  // Example uses of timing function
	timing(0, 3);
	timing(1, 0);
}
