%IR LED Light Intensity Curves
distance = [9.5,53.5,65.5,77,88,101.5,115.5,139.5,153]; %cm
ADCValue = [1023,1000,800,600,400,200,100,50,0]; % from 0 to 1024
distance1 = [10,20,30,40,50,60,70,80,90,100,110];
ADCValue1 = [1015,1003,991,991,983,975,711,522,250,152,48];
plot(distance, ADCValue,'k-')
hold on
p = polyfit(distance(4:8),ADCValue(4:8),2);
x = linspace(55,100,10);
y = polyval(p,x);
plot(x,y,'m*');
hold on

plot(distance1, ADCValue1,'b-')
xlabel('Distance (cm)','FontSize',14)
ylabel('ADC Reading (from 0 to 1024)','FontSize',14)
grid ON
hold on

p1 = polyfit(distance1(4:8),ADCValue1(4:8),2);
x1 = linspace(60,90,10);
y1 = polyval(p1,x1);
plot(x1,y1,'ro');

l=legend('Data Set 1', 'PolyFit for Data Set 1','Data Set 2', 'PolyFit for Data Set 2')
set(l,'FontSize',14)

% % Function to find two biggest values from an array
% ADC_Channels = [0,1,2,3,4,5,50,6];
% maxADC1 = 0;
% maxADC2 = 0;
% for z = 1:1:length(ADC_Channels)
%     if ADC_Channels(z)>maxADC1
%         maxChannel1 = z;
%         maxADC2 = maxADC1;
%         maxADC1 = ADC_Channels(z);
%     elseif ADC_Channels(z)>maxADC2
%             maxADC2 = ADC_Channels(z);
%             maxChannel2 = z;
% end
% end
%             
            
