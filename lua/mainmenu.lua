
ENGINE.IntroOn = false;
ENGINE.IntroBlackScreenAlpha = 0;

function ENGINE.StartIntro()
    
    ENGINE.IntroOn = true;
    
    Game.FadeVolume( 2000, 1, 0 );
    
    addTimer( 3000, function()
                        local t = addTimedText( FONT_MC, 32, "It is the year 2149.", 35000, 10, 10, 1, 1, 1, 1 );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 26000, 150 );
                        Game.PlayMusic( "intro.mp3", .1 );
                        Game.FadeVolume( 1000, .1, 1 );
                    end );
    
    addTimer( 6000, function()
                        t = addTimedText( FONT_MC, 32, "Two years ago, unidentified aggressors terrorized\nspace colonies located across the Byrgius and\nLangrenus sectors between the Earth and\nthe Moon.", 35000, 10, 100, 1, 1, 1, 1 );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 24000, 150 );
                    end );

    addTimer( 12000, function()
                        t = addTimedText( FONT_MC, 32, "Sporadic attacks evolved into full-on assaults with\nwaves of enemy combatants.  Eventually, war was\ndeclared against this unknown assailant known\notherwise as the \"UEA\".", 35000, 10, 300, 1, 1, 1, 1 );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 19000, 150 );
                    end );

    addTimer( 19000, function()
                        t = addTimedText( FONT_MC, 32, "To ramp up war efforts, a global draft was\nput into place.", 35000, 10, 500, 1, 1, 1, 1 );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 13000, 150 );
                    end );

    
    addTimer( 24000, function()
                        t = addTimedText( FONT_MC, 32, "Welcome to the UNSD.", 35000, Game.ScreenWidth() * .5, 650, 1, 1, 1, 1, true );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 9000, 150 );
                        Game.FadeVolume( 11000, 1, 0 );
                    end );
    
    addTimer( 36000, function()
                        t = addTimedText( FONT_MC, 32, "Time to blow shit up !", 35000, Game.ScreenWidth() * .5, Game.ScreenHeight() * .5, 1, 0, 0, 1, true, true );
                        addDelayToTimedText( t, 30 );
                        addFadeToTimedText( t, 3000, 150 );
                    end );
    
    addTimer( 43000, function() 
                        ENGINE.StateChange = "UPGRADESELECT";
                     end );
    
    
end

function ENGINE.MainMenuDraw()
    
    timerThink();
    
    if( ENGINE.IntroOn ) then
    
         Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 25, 25, 25, ENGINE.IntroBlackScreenAlpha );
        ENGINE.IntroBlackScreenAlpha = ENGINE.IntroBlackScreenAlpha + 100 * GetDelta();
        
        drawTimedText();
        
    end
    
end