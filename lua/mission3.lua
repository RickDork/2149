
LEVEL.BlackFadeOn = false;
LEVEL.BlackFadeStart = 0;
LEVEL.BlackFadeAlpha = 0;
LEVEL.BlackFadeRemove = false;
LEVEL.MarthusOffsetY = 300;
LEVEL.MarthusSpeechOn = false;

function LEVEL.Init()
    
    if( Game.GetRetryCount() > 0 ) then

        LEVEL.StartMission();
        return;
            
    end
    
    startScene();
    

    hookEvent( GetTicks() + 3000,
            function()
            
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Please give your attention to a UNSD-wide message from General Marthus.", 3000 );
        
                addTimer( 5000, function()
                    LEVEL.BlackFadeOn = true;
                    LEVEL.BlackFadeStart = GetTicks();
                    LEVEL.BlackFadeAlpha = 0;
                    end );
       
            
              --  addTimer( 1000, function() Game.PadTicks( 64000 ); end );
            
                addDialogueText( DIALOG_PROFILE_GASTON, "..Did he say eight years?", 3000, 46000 );
                addDialogueText( DIALOG_PROFILE_GASTON, "Must have been a mistake.", 3000, 49000 );
                addDialogueText( DIALOG_PROFILE_GASTON, "AI, repeat last message.", 3000, 52000 );
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Message not found.", 3000, 55000 );
                addDialogueText( DIALOG_PROFILE_GASTON, "What?  Retrieve last General Marthus message.", 3000, 58000 );
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Message not found.", 3000, 62000 );
                addDialogueText( DIALOG_PROFILE_GASTON, "..", 3000, 64000 );  
                addDialogueText( DIALOG_PROFILE_GASTON, "AI, what year is it?", 3000, 67000 );  
                addDialogueText( DIALOG_PROFILE_KALMIYA, "The year is 2149.", 3000, 70000 );
                addDialogueText( DIALOG_PROFILE_GASTON, "Patch me to unit 003 again.", 3000, 73000 ); 
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Communications link established with unit 003.", 3000, 78000 ); 
                addDialogueText( DIALOG_PROFILE_SQUADMATE, "What the hell do you need?", 3000, 82000 ); 
                addDialogueText( DIALOG_PROFILE_GASTON, "Marthus's last message... did he say the war's been going on for eight years?", 3000, 84000 ); 
                addDialogueText( DIALOG_PROFILE_SQUADMATE, "What? ..", 3000, 89000 ); 
                addDialogueText( DIALOG_PROFILE_SQUADMATE, "I think he did.", 3000, 91000 ); 
                addDialogueText( DIALOG_PROFILE_SQUADMATE, "Wait a secon-", 3000, 94000 ); 
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Communications link canceled.", 3000, 95000 ); 
                addDialogueText( DIALOG_PROFILE_GASTON, "What?", 3000, 97000 ); 
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Large enemy presence detected.  If you und-", 3000, 100000 ); 
                addDialogueText( DIALOG_PROFILE_GASTON, "Stop.  Disengage autopilot, this is outside the mission area.", 3000, 102000 ); 
                addDialogueText( DIALOG_PROFILE_KALMIYA, "Mission zone updated.  Objective updated.", 3000, 105000 ); 
            end, true );
    
    hookEvent( GetTicks() + 110000, 
            function()
            
                endScene();
            
            end, true );
    
end

LEVEL.HandledBossDeath = false;
LEVEL.WhiteFadeY = 0;
LEVEL.DrawBossDeath = false;
LEVEL.WhiteFadeYSpeed = 100;
LEVEL.DrawTalkScene = false;
LEVEL.TalkSceneAlpha = 0;
LEVEL.EnableTalkScene2 = false;

LEVEL.GastonX = 300;
LEVEL.GastonY = 450;
LEVEL.GastonW = 21;
LEVEL.GastonH = 57;
 
LEVEL.ScientistX = 450;
LEVEL.ScientistY = 300;
LEVEL.ScientistW = 30; 
LEVEL.ScientistH = 51;

function LEVEL.BossDeath()

    LEVEL.DrawBossDeath = true;
    
    if( not LEVEL.HandledBossDeath ) then
       
        LEVEL.HandledBossDeath = true;
        
        for n = 1, 30 do
           
            addTimer( n * 100, function() Game.CreateExplosions( 5, n * 50 + math.random( -50, 50 ), math.random( 0, 100 ) ) end );
            addTimer( n * 150, function() Game.CreateExplosions( 5, n * 50 + math.random( -50, 50 ), math.random( 60, 160 ) ) end );
            addTimer( n * 250, function() Game.CreateExplosions( 5, n * 50 + math.random( -50, 50 ), math.random( 60, 160 ) ) end );
            
        end
       
        Draw.SetDrawHUD( false );
        Game.TogglePlayerInput( false );
        
        Game.SetMaxEnemyCount( 0 );
        Game.ClearGenQueue();
        
        local t = GetTicks();
        
        LEVEL.ScientistW = LEVEL.ScientistW * 5;
        LEVEL.GastonW = LEVEL.GastonW * 5;
        LEVEL.ScientistH = LEVEL.ScientistH * 5;
        LEVEL.GastonH = LEVEL.GastonH * 5;
        
        addDialogueText( DIALOG_PROFILE_GASTON2, "... Am I dead?", 3000, 11000 );
        addDialogueText( DIALOG_PROFILE_SCIENTIST, "No.", 3000, 17000 );
        addTimer( 19000, function() LEVEL.DrawTalkScene = true; end );
        
        addDialogueText( DIALOG_PROFILE_GASTON2, "Where are we?", 3000, 23000 );
        addDialogueText( DIALOG_PROFILE_SCIENTIST, "On the cusp of the event horizon.", 3000, 28000 );
        addTimer( 28500, function() 
                LEVEL.FunkyBG = true; 
                LEVEL.EnableTalkScene2 = true;
            end );
        
    end

    
end

function LEVEL.Draw()
    
    if( LEVEL.BlackFadeOn and CutSceneOn ) then
       
        if( not LEVEL.BlackFadeRemove ) then
            LEVEL.BlackFadeAlpha = LEVEL.BlackFadeAlpha + 100 * GetDelta();
        else
            LEVEL.BlackFadeAlpha = LEVEL.BlackFadeAlpha - 100 * GetDelta();
            
            if( LEVEL.BlackFadeAlpha < 0 ) then
               
                LEVEL.BlackFadeOn = false;
                LEVEL.BlackFadeAlpha = 0;
                
            end
        end
        
        if( LEVEL.BlackFadeAlpha > 255 ) then
            LEVEL.BlackFadeAlpha = 255; 
        end
        
        Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 25, 25, 25, LEVEL.BlackFadeAlpha );
        
        if( GetTicks() - LEVEL.BlackFadeStart > 3000 ) then
           
            LEVEL.MarthusOffsetY = LEVEL.MarthusOffsetY - 100 * GetDelta();
            
        end
        
        if( LEVEL.MarthusOffsetY < 0 ) then
           
            LEVEL.MarthusOffsetY = 0;
            
            if( not LEVEL.MarthusSpeechOn ) then
  
                local i = addTimedText( FONT_MC, 24, "Greetings to all UNSD warriors today!  I just want to commend all of you on a job well done.  With all of your efforts we have retaken many of our colonies, and pushed back the UEA even further.\n\nI know it has been a difficult eight years and I know you want nothing more than to go home to your families, however, just know that your work today means a future for your friends and families for tomorrow..", 35500, Game.ScreenWidth() * .15, Game.ScreenHeight() * .25 + 140, 1, 1, 1, 1, false, false );
                addDelayToTimedText( i, 60 );
                addFadeToTimedText( i, 30500, 100 );
                
                fitTimedText( i, Game.ScreenWidth() * .8 );
                
                addTimer( 32000, function()

                        LEVEL.BlackFadeRemove = true;
                        
                        end );
                
                LEVEL.MarthusSpeechOn = true;
                
            end
            
        end
        
        Draw.DrawTexture( "marthus.png", Game.ScreenWidth() * .25, 40 - LEVEL.MarthusOffsetY, 256, 256, 255, 255, 255, LEVEL.BlackFadeAlpha );
        drawTimedText();
        
    end
   
    if( LEVEL.DrawBossDeath ) then
    
        Draw.DrawTexture( "whitefade.png", 0, 0, Game.ScreenWidth(), LEVEL.WhiteFadeY, 255, 255, 255, 255 );

        local whitealpha = ( LEVEL.WhiteFadeY / Game.ScreenHeight() ) * 255 * .75;

        if( LEVEL.FunkyBG ) then
            
            Draw.FunkyBackground();
            
        else
           
            Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 255, 255, 255, whitealpha );    
            
        end
        
        LEVEL.WhiteFadeY = LEVEL.WhiteFadeY + LEVEL.WhiteFadeYSpeed * GetDelta();    

        if( LEVEL.WhiteFadeY > Game.ScreenHeight() * .3 ) then
           
            LEVEL.WhiteFadeYSpeed = 300;
            
        end
        
    end
    
    if( LEVEL.DrawTalkScene ) then
       
        if( not LEVEL.EnableTalkScene2 ) then
        
            LEVEL.TalkSceneAlpha = LEVEL.TalkSceneAlpha + 70 * GetDelta();

            if( LEVEL.TalkSceneAlpha > 255 ) then

                LEVEL.TalkSceneAlpha = 255;

            end
            
        else
           
            LEVEL.TalkSceneAlpha = LEVEL.TalkSceneAlpha - 70 * GetDelta();

            if( LEVEL.TalkSceneAlpha < 0 ) then

                LEVEL.TalkSceneAlpha = 0;
        
                LEVEL.GastonW = -LEVEL.GastonW;
                LEVEL.ScientistW = -LEVEL.ScientistW;
                LEVEL.GastonX = 505;
                LEVEL.GastonY = 425;
                LEVEL.ScientistX = 250;
                LEVEL.ScientistY = 275;
                LEVEL.EnableTalkScene2 = false;
                LEVEL.ScientistW = LEVEL.ScientistW * 1.5;
                LEVEL.GastonW = LEVEL.GastonW * 1.5;
                LEVEL.ScientistH = LEVEL.ScientistH * 1.5;
                LEVEL.GastonH = LEVEL.GastonH * 1.5;
        

            end
            
        end
        
        Draw.DrawTexture( "scientistfull.png", LEVEL.ScientistX, LEVEL.ScientistY, LEVEL.ScientistW, LEVEL.ScientistH, 255, 255, 255, LEVEL.TalkSceneAlpha );
        Draw.DrawTexture( "gastonfull.png", LEVEL.GastonX, LEVEL.GastonY, LEVEL.GastonW, LEVEL.GastonH, 255, 255, 255, LEVEL.TalkSceneAlpha );
    end
        
end


function LEVEL.StartMission()

    if( Game.GetRetryCount() < 1 ) then
    
        presentChapter( "III", "The Broken Clock" );
        
    end
    
    hookEvent( 500,
            function()
            
                local i = addTimedText( FONT_MC, 32, "Enemies Within Range.", 3500, Game.ScreenWidth() * .5, Game.ScreenHeight() * .25, 1, 1, 1, 1, true, false );
                addDelayToTimedText( i, 30 );
            
            end );
    
    hookEvent( 1000, function() Game.GenerateEnemy( true, 9 ); end );
    
    hookEvent( 5000, 
            function() 
                for j=1,6 do
                        Game.GenerateEnemy( true, 5 );
                    end
                Game.SetMaxEnemyCount( 10 );
                Game.AddToGenQueue( 1, .6 );
            end );
    
    
     hookEvent( 20000, 
            function() 
                for j=1,4 do
                        Game.GenerateEnemy( true, 5 );
                    end
                Game.AddToGenQueue( 3, .2 );
                Game.SetMaxEnemyCount( 25 );
            end );   

     hookEvent( 35000, 
            function() 
                for j=1,4 do
                        Game.GenerateEnemy( true, 5 );
                    end
                Game.SetMaxEnemyCount( 25 );
            end );
    
     hookEvent( 45000, 
            function() 
                for j=1,2 do
                        Game.GenerateEnemy( true, 7 );
                    end
                Game.SetMaxEnemyCount( 5 );
                Game.AddToGenQueue( 8, .1 );
            end );
     hookEvent( 60000, 
            function() 
                Game.SetMaxEnemyCount( 20 );
            end );
     hookEvent( 90000, 
            function() 
                Game.SetMaxEnemyCount( 0 );
                Game.ClearGenQueue();
            end );
     hookEvent( 97000, 
            function() 
                if( Game.GetEnemyCount() < 1 ) then
                
                    addDialogueText( DIALOG_PROFILE_KALMIYA, "Warning, large enemy craft within range.", 3000 ); 
                    Game.GenerateEnemy( true, 9 );
                    return false;
                end
                return true;
            end, false, true );
    
    
end