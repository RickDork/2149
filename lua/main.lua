include( "scripts/lib.lua" );
include( "scripts/mainmenu.lua" );

FONT_MC = "Minecraftia-Regular.ttf";

DrawLetterBox = false;
DrawLetterBoxTransitionOff = false;
DrawLetterBoxOffset = 0;

function ENGINE.GameInit() 

    events = { };
    LEVEL = { };

    local missionScript = "scripts/mission" .. Game.GetCurrentMission() .. ".lua";
    include( missionScript );
    LEVEL.Init();
    
end

function ENGINE.GameLogic()
    
    if( LEVEL.Think ) then
       LEVEL.Think(); 
    end
    
    eventThink();
    timerThink();
    
    if( ProceedToNextMission ) then
       
        ProceedToNextMission = false;
        
        events = { };
        Game.GotoNextMission();
        
    end
    
    
end

ENGINE.a = { };
ENGINE.b = false;
ENGINE.c = { };

function ENGINE.Draw()
  
    if( DrawLetterBox ) then
       
        Draw.DrawTexture( "pixel.png", 0, -DrawLetterBoxOffset, Game.ScreenWidth(), Game.ScreenHeight() * .1, 0, 0, 0, 255 );
        Draw.DrawTexture( "pixel.png", 0, DrawLetterBoxOffset + Game.ScreenHeight() * .9, Game.ScreenWidth(), Game.ScreenHeight() * .15, 0, 0, 0, 255 );
        
    end
    
    if( DrawLetterBoxTransitionOff ) then
       
        DrawLetterBoxOffset = DrawLetterBoxOffset + 100 * GetDelta();
        
        if( DrawLetterBoxOffset > Game.ScreenHeight() * .15 ) then
           
            DrawLetterBoxOffset = 0;
            DrawLetterBox = false;
            DrawLetterBoxTransitionOff = false;
            
        end
        
    end
    
    if( LEVEL.Draw ) then
       LEVEL.Draw(); 
    end
    
    drawDialogue();
    drawTimedText();
    
    if( LEVEL.PostDraw ) then
       LEVEL.PostDraw(); 
    end
    
end