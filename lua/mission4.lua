
LEVEL.GastonDelay = 80;

function LEVEL.Init() 
   
    if( Game.GetRetryCount() >= 1 ) then
    
        LEVEL.StartMission();
        return;
        
    end   
    
    startScene();
    endScene();
    
end

LEVEL.OrbMessage = false;

function LEVEL.Think()
   

end

function LEVEL.Draw()
   
    Draw.DrawTexture( "pixel.png", 0, 0, Game.ScreenWidth(), Game.ScreenHeight(), 0, 0, 0, 255 );
    
end

function LEVEL.StartMission()

    if( Game.GetRetryCount() < 1 ) then
    
        presentChapter( "IV", "2149" );
        
    end
    
    


end