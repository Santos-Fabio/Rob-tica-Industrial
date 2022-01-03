%Montagem do Robo e sua simuulação
dh = [0 0 10 0;0 0 10 0];
R = SerialLink(dh,'name','Robo Planar');
q= [0 0];
R.plot(q);
%Porta Serial
s= serial('COM4');
fopen(s);%lembrar de fechar
exit = 1;
while(exit)
    %Interção Usuário
    fase = input('Qual a fase do projeto: ');
    x = input('coordenada x: ');
    y = input('coordenada y: ');
    if ((fase<=0 || fase >3) ||(x>20 || x<0) || (y<-20 || y>20))
        fase = 1;
        x = 20;
        y = 0;
        disp('Entrada Inválida! Manipulador retornará para posição inicial')
    end
    fase = num2str(fase);
    %cinematica inversa + conversão para graus
    T = transl(x,y,0);
    movimento= R.ikine(T,'q0',[0 0],'mask',[1 1 0 0 0 0]);
    angulos = rad2deg(movimento);
    theta1 = round(angulos(1,1),3)+90;
    theta2 = round(angulos(1,2),3)+90;
    t1 = num2str(theta1);
    t2 = num2str(theta2);
    %Interação Arduino
    envio = ""+fase+" "+t1+" "+t2
    fprintf(s,envio);
    exit = input('Aperte 0 para sair e 1 para continuar: ');
end
%Fechando Serial
fclose(s);

