clear;
border = melf_border();

y = [0 1 0];
for index=2:21
    x = [border(index-1) border(index) border(index+1)];
    plot(x, y);
    hold on;
end
