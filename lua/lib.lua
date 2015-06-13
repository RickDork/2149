
events = { }
timedText = { }
timers = { }
dialogue = { }

CutSceneOn = false;

ProceedToNextMission = false;

function ENGINE.SkipCutscene()
    
    if( not CutSceneOn ) then
        return;
    end
    
    events = { }
    timedText = { }
    timers = { }
    dialogue = { }
   
    endScene();
    
end

function LogTable( tbl )
   
    logStr = "Table output: [";
    for k, v in pairs( tbl ) do
        if( type( v ) == "table" ) then
            for n, m in pairs( v ) do
               logStr = logStr .. "\n" .. k .. "[" .. n .. "] = " .. m; 
            end
        else
            logStr = logStr .. "\n" .. k .. " = " .. v; 
        end
    end
        
    logStr = logStr .. "\n];";
    Log( logStr );
        
end

function hookEvent( time, fn, trueticks, constant ) 
    
    local starttime = TicksElapsed() + time;
    
    if( trueticks ) then
       
        starttime = time;
        
    end

    table.insert( events, { Time = time, StartTime = starttime, Callback = fn, Constant = constant or false, TrueTicks = trueticks or false, Triggered = false } );
    
end

function eventThink() 
    
    local gamet = TicksElapsed();
    local realt = GetTicks();
    
    for k, v in pairs( events ) do
       
        if( not v.Triggered or v.Constant ) then
           
            local t = gamet;
            
            if( v.TrueTicks ) then
                t = realt;
            end
           
            if( t > v.StartTime ) then
                   
                local ret = v.Callback();
                events[k].Triggered = true;
                
                if( v.Constant and not ret ) then
                    events[k].Constant = false;
                end
                
            end
                
            if( not events[k].Constant and events[k].Triggered ) then
               
                events[k] = nil;    
                    
            end
            
        end
        
    end
    
end

function addTimedText( font, size, text, duration, x, y, r, g, b, a, horiz, vert )

    local id = #timedText + 1;
    
    timedText[#timedText+1] = { Font = font, Size = size, Text = text,
                                  x = x, y = y, r = r, g = g, b = b, a = a,
                                  NextFlash = 0, Flashing = false, Delayed = false, Fade = false,
                                  Speed = 0, Showing = true, KillTime = GetTicks() + duration,
                                  CenterHoriz = horiz or false, CenterVert = vert or false, sound = false };
      
    return id;
    
end

function fitTimedText( i, width )
   
    local text = timedText[i].Text;
    local font = timedText[i].Font;
    local size = timedText[i].Size;
    
    local ftext = Game.MakeStringFit( text, width, font, size );
            
    timedText[i].Text = ftext;
    
end

function addSoundToTimedText( i )
   
    timedText[i].sound = true;
    
end


function addFadeToTimedText( i, delay, speed )
   
    timedText[i].FadeStart = GetTicks() + delay;
    timedText[i].FadeSpeed = speed;
    timedText[i].Fade = true;
    
end

function addFlashingToTimedText( i, speed )
   
    timedText[i].Flashing = true;
    timedText[i].NextFlash = GetTicks() + speed;
    timedText[i].Speed = speed;
    
end

function addDelayToTimedText( i, delay )
   
    timedText[i].DelayTime = delay;
    timedText[i].DelayNextChar = GetTicks() + delay;
    timedText[i].DelayPos = 0;
    timedText[i].Delayed = true;
    
end

DIALOG_DEFAULT_X = .7;
DIALOG_DEFAULT_Y = .7;
DIALOG_DEFAULT_WIDTH = .45;
DIALOG_PROFILE_GASTON = "playerprofile.png";
DIALOG_PROFILE_GASTON2 = "playerprofile2.png";
DIALOG_PROFILE_SCIENTIST = "scientistprofile.png";
DIALOG_PROFILE_KALMIYA = "aiprofile.png";
DIALOG_PROFILE_SQUADMATE = "squadmateprofile.png";
DIALOG_PROFILE_DAD = "dadprofile.png";
DIALOG_PROFILE_DAUGHTER = "daughterprofile.png";
DIALOG_FONT_DELAY = 40;
DIALOG_FONT_SIZE = 24;

FinaleDialogueToggle = false;

function addDialogueText( profile, text, delay, initialdelay, size, textdelay )
   
    initialdelay = initialdelay or 0;
    
    if( FinaleDialogueToggle ) then
       textdelay = 75;
        DIALOG_DEFAULT_Y = .85;
    end
    
    addTimer( initialdelay, function() 

        text = Game.MakeStringFit( text, Game.ScreenWidth() * DIALOG_DEFAULT_WIDTH - 130, FONT_MC, DIALOG_FONT_SIZE );
            
        local l = string.len( text );
        local endtime = DIALOG_FONT_DELAY * l + delay;
        local x = Game.ScreenWidth() * DIALOG_DEFAULT_X - 50;
        local y = Game.ScreenHeight() * DIALOG_DEFAULT_Y;

        local rBack = 15;
        local gBack = 25;
        local bBack = 37;
        
        local rFore = 14;
        local gFore = 45;
        local bFore = 63;
            
        if( profile == DIALOG_PROFILE_GASTON or profile == DIALOG_PROFILE_GASTON2 ) then
            
            rBack = 34;
            gBack = 34;
            bBack = 34;
                
            rFore = 51;
            gFore = 51;
            bFore = 51;
                
        end
            
        if( profile == DIALOG_PROFILE_SQUADMATE ) then
            
            rBack = 111;
            gBack = 77;
            bBack = 0;
                
            rFore = 121;
            gFore = 87;
            bFore = 21;
                
        end
            
        if( profile == DIALOG_PROFILE_SCIENTIST ) then
                
            rBack = 107;
            gBack = 26;
            bBack = 26;
                
            rFore = 107;
            gFore = 26;
            bFore = 26;
                
        end
            
        for k, v in pairs( dialogue ) do

            dialogue[k].yoffset = dialogue[k].yoffset + 150;
            dialogue[k].y = dialogue[k].y - 150;
            dialogue[k].timedText.y = dialogue[k].timedText.y - 150;

                    
        end
            
        local delayadd = 0;
            
        if( table.getn( dialogue ) > 0 ) then
            delayadd = 300;
        end

        addTimer( delayadd, function() 

                                local t = addTimedText( FONT_MC, size or DIALOG_FONT_SIZE, text, endtime, x, y, 1, 1, 1, 1 );

                                addDelayToTimedText( t, textdelay or DIALOG_FONT_DELAY );

                                timedText[t].DoNotDraw = true;
                                timedText[t].DelayNextChar = timedText[t].DelayNextChar + 400;
                                timedText[t].sound = true;

                                table.insert( dialogue, { timedTextId = t, timedText = timedText[t], profileImage = profile, endTime = endtime + GetTicks(), x = x, y = y, yoffset = 100, a = 0, offsetSpeed = 270, rBack = rBack, gBack = gBack, bBack = bBack, rFore = rFore, gFore = gFore, bFore = bFore, Text = text } );
                    
                    
                            end );

    end );

end

function drawDialogue() 

    for k, v in pairs( dialogue ) do
       
        if( v.a >= 0 ) then

            local t = v.timedText;

            if( t ) then

                local width = Game.ScreenWidth() * DIALOG_DEFAULT_WIDTH;
                --Draw.DrawTexture( "pixel.png", v.x - 150, v.y - 14 + v.yoffset, width, 128, v.rBack, v.gBack, v.bBack, v.a );
                
                if( not FinaleDialogueToggle ) then
                    Draw.DrawTexture( "pixel.png", v.x - 16, v.y - 8 + v.yoffset, width - 120, 145, v.rFore, v.gFore, v.bFore, v.a );
                end
                
                local miscoffset = 0;
                
                if( FinaleDialogueToggle ) then
                   
                    miscoffset = 35;
                    
                end
                
                Draw.DrawTexture( v.profileImage, v.x - 160, v.y - 8 + v.yoffset - miscoffset, 138, 145, 255, 255, 255, v.a );

                
                v.timedText.KillTime = t.KillTime + 1000;

                if( t ) then
                   Draw.DrawString( t.Font, t.Size, t.Text:sub( 1, t.DelayPos ), t.x, t.y + v.yoffset, t.r, t.g, t.b, v.a / 255, t.CenterHoriz, t.CenterVert ); 
                end

                if( v.a < 255 and GetTicks() < v.endTime ) then
                    v.a = v.a + 250 * GetDelta();
                    if( v.a > 255 ) then
                        v.a = 255;
                    end
                end

                if( v.yoffset > 0 ) then
                    v.yoffset = v.yoffset - v.offsetSpeed * GetDelta();
                    if( v.yoffset < 0 ) then
                        v.yoffset = 0; 
                    end
                end

                if( GetTicks() > v.endTime ) then

                    dialogue[k].a = dialogue[k].a - 400 * GetDelta();

                end
                
            else
                dialogue[k] = nil; 
            end

        else
           timedText[v.timedTextId] = nil;
           dialogue[k] = nil; 
        end
       
        
    end
    
end

function drawTimedText() 
   
    for k, v in pairs( timedText ) do
        
        local l = v.Text:len();

        if( v.Delayed ) then

            l = v.DelayPos;

            if( l < v.Text:len() ) then

                if( GetTicks() > v.DelayNextChar ) then

                    timedText[k].DelayNextChar = v.DelayTime + GetTicks();
                    timedText[k].DelayPos = v.DelayPos + 1;
                    
                    if( v.sound ) then
                        Game.PlaySound( "bullet.wav", .3 );
                    end
                    
                end

            end

        end
           
        if( v.Showing ) then
        
            if( not v.DoNotDraw ) then
                Draw.DrawString( v.Font, v.Size, v.Text:sub( 1, l ), v.x, v.y, v.r, v.g, v.b, v.a, v.CenterHoriz, v.CenterVert );
            end
            
        end
       
        if( v.Flashing and GetTicks() > v.NextFlash ) then
           
            timedText[k].NextFlash = GetTicks() + timedText[k].Speed;
            timedText[k].Showing = not timedText[k].Showing;
            
        end
        
        if( v.Fade ) then
           
            if( GetTicks() > v.FadeStart and v.a > 0 ) then
               
                local a = v.a * 255;
                a = a - v.FadeSpeed * GetDelta();
                
                if( a < 0 ) then
                    a = 0;
                end
                
                timedText[k].a = a / 255;
                
            end
            
        end
        
        if( GetTicks() > v.KillTime ) then
        
            timedText[k] = nil;
            
        end
        
    end
    
end

function addTimer( delay, cb ) 
    
    table.insert( timers, { Delay = delay + GetTicks(), Callback = cb } );
        
end
    
function timerThink() 
   
    for k, v in pairs( timers ) do
       
        if( GetTicks() > v.Delay ) then
           
            v.Callback();
            timers[k] = nil;

        end
        
    end
    
end

function nextMission()
   
    ProceedToNextMission = true;
    
end


function startScene()
   
    FreezeGameTicks();
    Draw.SetDrawHUD( false );
    Game.TogglePlayerInput( false );
    DrawLetterBox = true;
    DrawLetterBoxTransitionOff = false;
    
    Game.ToggleScene( true );
    CutSceneOn = true;
    
end

function endScene()
   
    if( not CutSceneOn ) then
        return;
    end
    
    Draw.SetDrawHUD( true );
    DrawLetterBoxTransitionOff = true;
    LEVEL.StartMission();
    Game.TogglePlayerInput( true );
    
    Game.ToggleScene( false );
    CutSceneOn = false;
    
end

function checkEndMission() 
   
        if( Game.GetEnemyCount() < 1 ) then
                    
            addTimer( 2000, function()

                                local i = addTimedText( FONT_MC, 32, "MISSION COMPLETE", 4000, Game.ScreenWidth() * .5, Game.ScreenHeight() * .25, 1, 1, 1, 1, true, false );
                                addDelayToTimedText( i, 120 );
                                FreezeGameTicks();

                                addTimer( 5000, function() nextMission(); end );

                            end );

            return true;

        end
    
    return false;
    
end

function presentChapter( num, title )

    FreezeGameTicks();

    local width = Game.GetStringWidth( FONT_MC, 72, title );
    
    hookEvent( GetTicks() + 1000,
            function()
            
                local missiontext = "Mission " .. num .. ".";
            
                if( num == "IV" ) then
                
                    missiontext = "Finale."
                
                end

                local i = addTimedText( FONT_MC, 32, missiontext, 6800, Game.ScreenWidth() * .5 - width * .5, Game.ScreenHeight() * .35, 1, 1, 1, 1, false, false );
                addFadeToTimedText( i, 4800, 130 );

            end, true );

    hookEvent( GetTicks() + 2800,
            function()

                local i = addTimedText( FONT_MC, 72, title, 5000, Game.ScreenWidth() * .5 - width * .5, Game.ScreenHeight() * .35 + 50, 1, 1, 1, 1, false, false );
                addFadeToTimedText( i, 3000, 130 );
                addDelayToTimedText( i, 90 );     

            end, true );

    hookEvent( GetTicks() + 8000,
            function()

                UnfreezeGameTicks();

            end, true );
    
end
