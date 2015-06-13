
LEVEL.GastonDelay = 80;

function LEVEL.Init() 
   
    if( Game.GetRetryCount() >= 1 ) then
    
        LEVEL.StartMission();
        return;
        
    end   
    
    startScene();
    endScene();
    
    LEVEL.BlackScreenAlpha = 255;
    LEVEL.StartTicks = GetTicks();
    
end

LEVEL.OrbMessage = false;

function LEVEL.Think()
   

end

FlashFaceImg = "";
NextFlashFace = 0;
FlashFaceBG = true;
FlashFaceBGToggle = false;
FlashFaceEnd = 0;
FlashR = 255;
FlashG = 255;
FlashB = 255;

local function FlashFace() 
   
    if( GetTicks() > NextFlashFace ) then
        
        NextFlashFace = GetTicks() + 30;
        FlashFaceBGToggle = not FlashFaceBGToggle;
        
        local r = math.random( 1, 3 );

        if( r == 1 ) then

            FlashFaceImg = "daughterprofile.png";

        elseif( r == 2 ) then

            FlashFaceImg = "dadprofile.png";

        elseif( r == 3 ) then

            FlashFaceImg = "scientistprofile2.png";

        end
        
    end
    
    if( FlashFaceBG and FlashFaceBGToggle ) then
        Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), FlashR, FlashG, FlashB, 200 );
    end
    
    Draw.DrawTexture( FlashFaceImg, 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 255, 255, 255, 180 );
    
end

LEVEL.EndingOn = false;
LEVEL.EndTime = 0;
LEVEL.CreditsOn = false;
LEVEL.CreditsSong = false;

function LEVEL.Draw()
    
    DrawLetterBox = true;
    
    if( LEVEL.CreditsOn ) then
       
        if( GetTicks() > LEVEL.EndTime + 7000 ) then
           
            creditString = "";
            
            if( GetTicks() > LEVEL.EndTime + 7000 and GetTicks() < LEVEL.EndTime + 12000 ) then
                creditString = "2149";
                if( not LEVEL.CreditsSong ) then
                    Game.PlayMusic( "map.mp3" );
                    LEVEL.CreditsSong =  true;
                end
            end
            
            if( GetTicks() > LEVEL.EndTime + 13000 and GetTicks() < LEVEL.EndTime + 18000 ) then
               
                creditString = "Developed by Rick Chou";
                
            end
            
            if( GetTicks() > LEVEL.EndTime + 19000 ) then
               
                creditString = "Thanks for playing!";
                
            end
            
            Draw.DrawString( FONT_MC, 24, creditString, Game.ScreenWidth() * .5, Game.ScreenHeight() * .5, 255, 255, 255, 255, true, true );
            
     
        end
        
    end
    
    if( LEVEL.EndingOn ) then
        return;
    end
   
    Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 0, 0, 0, LEVEL.BlackScreenAlpha );
    
    if( GetTicks() - LEVEL.StartTicks > 2500 and LEVEL.BlackScreenAlpha > 0 ) then
       
        LEVEL.BlackScreenAlpha = LEVEL.BlackScreenAlpha - 30.0 * GetDelta();
        
        if( LEVEL.BlackScreenAlpha < 0 ) then
            LEVEL.BlackScreenAlpha = 0;
        end
        
    end
    
end

function LEVEL.PostDraw()
    
    if( LEVEL.EndingOn ) then
        return;
    end
    
    if( GetTicks() < FlashFaceEnd ) then
        FlashFace();
    end

    
end

function LEVEL.StartMission()

    if( Game.GetRetryCount() < 1 ) then
    
        presentChapter( "IV", "2149" );
        
    end
    
    Game.PlayMusic( "ACSF.mp3" );
    
    Draw.SetDrawHUD( false );

    FinaleDialogueToggle = true;
    
    addDialogueText( DIALOG_PROFILE_SCIENTIST, "We were born into the cusp of a black hole.", 10000, 16000 );
    addDialogueText( DIALOG_PROFILE_SCIENTIST, "Ever flowing beyond the event horizon..", 10000, 35000 );
    addDialogueText( DIALOG_PROFILE_DAUGHTER, "Daddy, I love you! Come home soon!", 10000, 60000 );
    addDialogueText( DIALOG_PROFILE_SCIENTIST, "Terrified, climbing into the vast infinite.", 10000, 80000 );
    addDialogueText( DIALOG_PROFILE_DAD, "I\'m proud of you, son.", 10000, 105000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "What year is it?", 5000, 120000 );
    addDialogueText( DIALOG_PROFILE_SCIENTIST, "Can you still hear us?", 5000, 130000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 144000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 146000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 148000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 150000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 152000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 154000 );
    addDialogueText( DIALOG_PROFILE_GASTON2, "WHAT YEAR IS IT?", 20000, 156000 );
    
    hookEvent( 5000,
                function()
                    Game.SetMaxEnemyCount( 5 );
                    Game.AddToGenQueue( 10, .1 );
                    FlashFaceEnd = GetTicks() + 500;
            
                end );

    hookEvent( 25000,
                function()
                    Game.AddToGenQueue( 13, .1 );
                    FlashFaceEnd = GetTicks() + 200;
                end );
    
    hookEvent( 50000,
                function()
                    Game.AddToGenQueue( 12, .1 );
                    Game.SetMaxEnemyCount( 20 );
                    FlashFaceEnd = GetTicks() + 1500;
                end );
    
    hookEvent( 70000,
                function()
                    Game.AddToGenQueue( 14, .1 );
                end );
    
    hookEvent( 100000,
                function()
                Game.AddToGenQueue( 11, .1 );
                Game.SetMaxEnemyCount( 50 );
                FlashFaceEnd = GetTicks() + 500;
                end );

    hookEvent( 142000,
                function()
                Game.FadeVolume( 6000, 1, 5 );
                end );
    
    hookEvent( 145000,
                function()
                FlashG = 0;
                FlashB = 0;
                FlashFaceEnd = GetTicks() + 7000;
                Game.TogglePlayerInput( false );
                end );
    
    hookEvent( 148000,
                function()
                Game.TriggerEnding();
                events = { }
                timedText = { }
                timers = { }
                dialogue = { }
                LEVEL.EndingOn = true;
                addTimer( 700, function()
                    
                        Game.ExplodePlayer();
                        LEVEL.CreditsOn = true;
                        LEVEL.EndTime = GetTicks();
                    
                    end );
                end );
    
    
end