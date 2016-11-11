figure
hold on
axis equal
bot_r = 3.81; %bot radius in cm

% Draw Court
rectangle('Position',[0 0 230 120],'Curvature',[59/230, 59/120])
% Draw Goals
rectangle('Position',[-5 29.5 5 59.7])
rectangle('Position',[230 29.5 5 59.7])

load('C.mat');
for i = 1: length(rawStarData)
    X1 = rawStarData(i,1); X2 = rawStarData(i,2);
    X3 = rawStarData(i,3); X4 = rawStarData(i,4);
    Y1 = rawStarData(i,5); Y2 = rawStarData(i,6);
    Y3 = rawStarData(i,7); Y4 = rawStarData(i,8);
    if (X1 == 1023 && X2 ~= 1023 && X3 ~= 1023 && X4 ~= 1023)
        [x, y, t] = sees3(X2, Y2, X3, Y3, X4, Y4);
    elseif (X1 ~= 1023 && X2 == 1023 && X3 ~= 1023 && X4 ~= 1023)
        [x, y, t] = sees3(X1, Y1, X3, Y3, X4, Y4);
    elseif (X1 ~= 1023 && X2 ~= 1023 && X3 == 1023 && X4 ~= 1023)
        [x, y, t] = sees3(X1, Y1, X2, Y2, X4, Y4);
    elseif (X1 ~= 1023 && X2 ~= 1023 && X3 ~= 1023 && X4 == 1023)
        [x, y, t] = sees3(X1, Y1, X2, Y2, X3, Y3);
    elseif (X1 == 1023 || X2 == 1023 || X3 == 1023 || X4 == 1023)
        disp('2 or fewer stars');
        x = 0; y = 0;
    else
        [x, y, t] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4);
    end
    if (x ~= 0 && y ~= 0)
        x = x + 230/2;
        y = y + 120/2;
        plot(x, y, 'mo', 'MarkerSize', bot_r*2);
        quiver(x, y, bot_r * cos(t), bot_r * sin(t));
    end
    pause(0.001);
end
hold off