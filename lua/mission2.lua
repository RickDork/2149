

function LEVEL.Init() 
   
    if( Game.GetRetryCount() > 0 ) then
    
        LEVEL.StartMission();
        return;
        
    end   
    
    Game.PlayMusic( "rtj2.mp3" );
    
    startScene();
     
    hookEvent( GetTicks() + 3000,
            function()
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Enemy presence no longer detected.", 6000 );
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Please take this time to ensure that all safety mechanisms are activated and that you-", 2000, 3000 );
            
                addDialogueText( DIALOG_PROFILE_GASTON, "Mute.", 3000, 7000 );
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Unrecognized command.", 2000, 9000 );

                addDialogueText( DIALOG_PROFILE_GASTON, "Mute.  Mute voice.  Silence.", 3000, 11000 );

                addDialogueText( DIALOG_PROFILE_KALMIYA, "Please say \"HELP\" to retrieve a list of possible commands.", 3000, 14500 );

                addDialogueText( DIALOG_PROFILE_GASTON, "...", 3000, 18000 );
            
                addDialogueText( DIALOG_PROFILE_GASTON, "Create a comms link with the nearest squadmate.", 3000, 21000 );

                addDialogueText( DIALOG_PROFILE_KALMIYA, "Communications link established with unit 003.", 3000, 28000 );

                addDialogueText( DIALOG_PROFILE_SQUADMATE, "What do you need?", 3000, 31000 );

                addDialogueText( DIALOG_PROFILE_GASTON, "Hey.. what is the command to mute the AI?", 3000, 34000 );

                addDialogueText( DIALOG_PROFILE_SQUADMATE, "Fuck off.", 3000, 40000 ); 
                addDialogueText( DIALOG_PROFILE_GASTON, "What\'s y-", 3000, 41500 );

                addDialogueText( DIALOG_PROFILE_KALMIYA, "Communications link terminated.", 3000, 42500 );

                addDialogueText( DIALOG_PROFILE_GASTON, "What a shithead.", 3000, 46000 );

                addDialogueText( DIALOG_PROFILE_KALMIYA, "Additional enemy presence has been detected.  If you understand, please conf-", 3000, 50000 );

                addDialogueText( DIALOG_PROFILE_GASTON, "Understood!", 3000, 54000 );


            end, true );
    
    hookEvent( GetTicks() + 63000, 
            function()
            
                endScene();
            
            end, true );
    
end

function LEVEL.StartMission()

    if( Game.GetRetryCount() < 1 ) then
    
        presentChapter( "II", "Golden Silence." );
        
    end

    hookEvent( 2000,
            function()
            
                local i = addTimedText( FONT_MC, 32, "Enemies Within Range.", 3500, Game.ScreenWidth() * .5, Game.ScreenHeight() * .25, 1, 1, 1, 1, true, false );
                addDelayToTimedText( i, 30 );

            end );
    
    hookEvent( 5000, 
            function() 
                Game.SetMaxEnemyCount( 7 );
                Game.AddToGenQueue( 3, .4 );
            end );
    
    hookEvent( 25000,
                function()
                    Game.SetMaxEnemyCount( 19 );
                    Game.AddToGenQueue( 1, .1 );
                    Game.AddToGenQueue( 2, .2 );
                end );
    

    hookEvent( 35000,
                function()
                    Game.SetMaxEnemyCount( 40 );
                end );

    hookEvent( 40000,
                function()
                    for j=1,3 do
                        Game.GenerateEnemy( true, 6 );
                    end
                    Game.SetMaxEnemyCount( 7 );
                end );

    
    hookEvent( 42000,
                function()
                    for j=1,3 do
                        Game.GenerateEnemy( true, 5 );
                    end
                    Game.SetMaxEnemyCount(  5 );
                end );
    
    hookEvent( 55000,
                function()
                    for j=1,3 do
                        Game.GenerateEnemy( true, 6 );
                    end
            
                    for j=1,2 do
                        Game.GenerateEnemy( true, 5 );
                    end
                    Game.SetMaxEnemyCount( 12 );
                end );
    
    hookEvent( 75000, 
                function()
                    Game.ClearGenQueue();
                    
                    for j=1,45 do
                        Game.GenerateEnemy( false, 4 );
                    end
            
                end );
    
    hookEvent( 80000, 
                function()
                    for j=1,3 do
                        Game.GenerateEnemy( true, 6 );
                    end
                end );
    
    hookEvent( 84000, 
                function()
                    for j=1,3 do
                        Game.GenerateEnemy( true, 5 );
                    end
                end );
    
    hookEvent( 90000, 
        function() 
            
                if( checkEndMission() ) then
                
                    return false;
                
                end
            
                return true;
                    
        end, false, true );

end