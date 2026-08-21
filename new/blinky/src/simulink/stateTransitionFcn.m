function x_next = stateTransitionFcn(x, u)
    %#codegen
    % x = [pitch; roll]  (2×1)
    % u = [gx; gy]       (2×1)
    
    pitch = x(1);
    roll  = x(2);
    gx = u(1);
    gy = u(2);
    dt = 0.005;
    
    g_pitch = pitch + gy * dt;
    g_roll  = roll + gx * dt;
    
    x_next = [g_pitch; g_roll];   % 2×1
end