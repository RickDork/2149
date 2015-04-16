
function NewSprite( texture ) 

    local animData = Draw.GetAnimData( texture );

    local spr = {
    
        Texture = texture,
        FramePause = { },
        NextFrameTime = 0,
        MaxFrames = animData.MaxFrames,
        CurFrame = 1
    
    };
    
    for k, v in pairs( animData.FramePause ) do
       
        spr.FramePause[k] = v;
        
    end
    
    spr.NextFrameTime = spr.FramePause[1] + GetTicks();
    
    spr.Draw = function( this, x, y, w, h, r, g, b, a )

        if( this.MaxFrames > 1 ) then
        
            if( GetTicks() > this.NextFrameTime ) then

                this.CurFrame = this.CurFrame + 1;

                if( this.CurFrame > this.MaxFrames ) then
                    this.CurFrame = 1;
                end

                this.NextFrameTime + GetTicks() + this.FramePause[this.CurFrame];

            end
            
        end

        Draw.DrawTexture( this.Texture, x, y, w, h, r, g, b, a, this.CurFrame );
    
    end
    
    return spr;

end