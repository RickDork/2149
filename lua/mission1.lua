
function LEVEL.Init() 
   
    if( Game.GetRetryCount() > 0 ) then

        LEVEL.StartMission();
        return;
        
    end   
    
    Game.PlayMusic( "rtj.mp3" );
    
    startScene();
    

    hookEvent( GetTicks() + 3000,
            function()
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Salutations.  For your reference, I am Kalmiya, a voice-automated artificial intelligence.", 3000 );
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "My goal is to provide immediate assistance and combat field information.", 3000, 6000 );
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "If you understand, please confirm by saying \"Understood\".", 3000, 11000 );
            
                addDialogueText( DIALOG_PROFILE_GASTON, "Understood.", 3000, 16000 );
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Thank you.  Have a safe mission.", 2000, 18000 );
            
            end, true );
    
    hookEvent( GetTicks() + 25000, 
            function()
            
                endScene();
            
            end, true );
    
end


function LEVEL.Think()
  
    if( not Game.IntroControlMessage ) then

		if( not CutSceneOn ) then
	
			Game.IntroControlMessage = true;
		
			addTimer( 11000, function()
			
				Game.ToggleGameFrozen( true );
				
				addDialogueText( DIALOG_PROFILE_KALMIYA, "Use WASD to move your ship around.  Press ESCAPE to pause or skip cutscenes.", 4000 );

				addDialogueText( DIALOG_PROFILE_KALMIYA, "Your guns will automatically fire when enemies are detected.", 2000, 6000 );

				addTimer( 11000, function()

									Game.ToggleGameFrozen( false );            

								end );
					
			end );

		end

    end
    
   
    if( not Game.IntroOrbMessage and Game.IntroControlMessage ) then

        if( Game.GetPlayerKillCount() > 0 ) then
        
            Game.IntroOrbMessage = true;
			
            addTimer( 900, function()
            
                Game.ToggleGameFrozen( true );
                
                addDialogueText( DIALOG_PROFILE_KALMIYA, "When enemies are destroyed, they will expel blue and green colored orbs.", 2000 );

                addDialogueText( DIALOG_PROFILE_KALMIYA, "Collect these for experience and health.", 2000, 4000 );

                addTimer( 7000, function()

                                    Game.ToggleGameFrozen( false );            

                                end );
                    
            end );
            
        end
		
	end
		
    if( not Game.IntroTipMessage ) then
		
		if( not CutSceneOn ) then
			
			Game.IntroTipMessage = true;
			
			hookEvent( 60000, function() 
		
				Game.ToggleGameFrozen( true );
				
				addDialogueText( DIALOG_PROFILE_KALMIYA, "Instead of primarily focusing on dodging and avoiding enemies.. ", 2000 );
				addDialogueText( DIALOG_PROFILE_KALMIYA, "You may find it advantageous to be more proactive and on the offensive.", 2000, 4000 );
				addDialogueText( DIALOG_PROFILE_KALMIYA, "Higher kills warrant larger yields of health and experience.", 2000, 8000 );

				addTimer( 12000, function()

									Game.ToggleGameFrozen( false );            

								end );
					
			end );
		
		end


    end
    
end

function LEVEL.StartMission()

    if( Game.GetRetryCount() < 1 ) then
    
        presentChapter( "I", "Understand?" );
        
    end
    
    hookEvent( 3000,
            function()
            
                local i = addTimedText( FONT_MC, 32, "Enemies Within Range.", 3500, Game.ScreenWidth() * .5, Game.ScreenHeight() * .25, 1, 1, 1, 1, true, false );
                addDelayToTimedText( i, 30 );
            
            end );
    
    hookEvent( 5000, 
            function() 
                Game.SetMaxEnemyCount( 5 );
                Game.AddToGenQueue( 0, .3 );
            end );


    hookEvent( 15000, function() Game.SetMaxEnemyCount( 10 ); end );

    hookEvent( 25000, 
        function() 
            for j=1,3 do
               Game.CreateEnemyFormation( false, 1, 3 ); 
            end
        end );

    hookEvent( 35000, function() Game.SetMaxEnemyCount( 10 ); Game.AddToGenQueue( 2, .2 ); end );

    hookEvent( 48000, 
        function() 
            Game.SetMaxEnemyCount( 15 );
            for j=1,7 do
               Game.CreateEnemyFormation( false, 1, 3 ); 
            end
        end );

    hookEvent( 65000, 
        function() 
            Game.SetMaxEnemyCount( 20 );
            Game.AddToGenQueue( 3, .1 );
            for j=1,3 do
               Game.CreateEnemyFormation( false, 1, 3 ); 
            end
        end );

    hookEvent( 85000, 
        function() 
            Game.SetMaxEnemyCount( 15 );
            for j=1,10 do
               Game.CreateEnemyFormation( false, 1, 3 ); 
            end
        end );

    hookEvent( 90000, 
        function() 
            Game.SetMaxEnemyCount( 0 );
        end );
    
    hookEvent( 95000, 
        function() 
            
                if( checkEndMission() ) then
                
                    return false;
                
                end
                    
                return true;
                    
        end, false, true );
    


end