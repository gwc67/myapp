function x_next = stateTransitionFcn(x, u)

    %#codegen
    
    %x = [pitch;roll;bias_gx;bias_gy]  只有加速度计下的pitch；roll;
    %biax_gx;bias_y; 角速度的零漂值
    % params = [gx;gy;dt] (陀螺仪的原始数据)

    

    pitch = x(1);
    roll = x(2);
    bias_gx = x(3);
    bias_gy = x(4);

    gx = u(1);
    gy = u(2);
    
    dt = 0.005;

    %陀螺仪积分（减去估计的零偏）
    pitch_next = pitch + (gy - bias_gy) * dt;
    roll_next  = roll + (gx - bias_gx) * dt;

    % 零偏假设为随机游走
    bias_gx_next = bias_gx;
    bias_gy_next = bias_gy;

    x_next = [pitch_next;roll_next;bias_gx_next;bias_gy_next];
end 