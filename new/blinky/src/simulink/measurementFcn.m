function z = measurementFcn(x)
    % 观测函数：状态 → 预测的观测值
    % EKF 内部用这个和 y1 比较，算残差
    % 估算值
    pitch = x(1);
    roll  = x(2);
    
    z = [pitch; roll];
end