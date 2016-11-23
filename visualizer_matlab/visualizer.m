%% Variables for Shift
thetaShiftMult = 100;

%% Configure Visualization
figure(1)
clf
hold on
axis equal
bot_r = 3.81; %bot radius in cm
% Draw Court
rectangle('Position',[0 0 230 120],'Curvature',[59/230, 59/120])
% Draw Goals
rectangle('Position',[-5 29.5 5 59.7])
rectangle('Position',[230 29.5 5 59.7])

%% Define Bot initial positions, color
B1 = [0 0 0];    
B2 = [0 0 0];
B3 = [0 0 0];
B1_color = 'm';
B2_color = 'o';
B3_color = 'g';

%% Initialize I/O
handle = serial('/dev/tty.usbmodem411','Baudrate', 9600);
fopen(handle);
fprintf(handle, message);
buffer = strsplit(message);

%% Handle Input
if (buffer(1) == 1) % checks if recieving robot input
    switch(buffer(2)) 
        case(1) % offensive bot 1
            B1 = [ buffer(3), buffer(4), buffer(5) ];
            x = B1(1) + 230/2;
            y = B1(2) + 120/2;
            t = B1(3) / thetaShiftMult;
            plot(x, y, 'o', 'MarkerFaceColor', B1_color , ...
                'MarkerEdgeColor', B1_color, 'MarkerSize', bot_r * 2);
            line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
        case(2) % offensive bot 2
            B2 = [ buffer(3), buffer(4), buffer(5) ];
            x = B2(1) + 230/2;
            y = B2(2) + 120/2;
            t = B2(3) / thetaShiftMult;
            plot(x, y, 'o', 'MarkerFaceColor', B2_color , ...
                'MarkerEdgeColor', B2_color, 'MarkerSize', bot_r * 2);
            line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
        case(3) % goalie bot
            B3 = [ buffer(3), buffer(4), buffer(5) ];
            x = B3(1) + 230/2;
            y = B3(2) + 120/2;
            t = B3(3) / thetaShiftMult;
            plot(x, y, 'o', 'MarkerFaceColor', B3_color , ...
                'MarkerEdgeColor', B3_color, 'MarkerSize', bot_r * 2);
            line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
    end
end
            

hold off