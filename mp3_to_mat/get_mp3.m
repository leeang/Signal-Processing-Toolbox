word{1} = 'one';
word{2} = 'two';
word{3} = 'three';
word{4} = 'four';
word{5} = 'five';
word{6} = 'six';
word{7} = 'seven';
word{8} = 'eight';
word{9} = 'nine';
word{10} = 'ten';
word{11} = 'eleven';
word{12} = 'twelve';
word{13} = 'thirteen';
word{14} = 'fourteen';
word{15} = 'fifteen';
word{16} = 'sixteen';
word{17} = 'seventeen';
word{18} = 'eighteen';
word{19} = 'nineteen';
word{20} = 'twenty';
word{21} = 'zero';
word{22} = 'on';
word{23} = 'off';
word{24} = 'channel';
word{25} = 'switch';
word{26} = 'volume up';
word{27} = 'volume down';

options = weboptions('Timeout', 20);
for word_index = 1
	% url = ['http://fanyi.baidu.com/gettts?lan=en&text=' word{word_index}];
	% url = ['https://ssl.gstatic.com/dictionary/static/sounds/de/0/' word{word_index} '.mp3'];
	url = ['http://dict.youdao.com/dictvoice?type=3&audio=' word{word_index}];
	filename = ['youdao/type3-' num2str(word_index) '.mp3'];
	outfilename = websave(filename, url, options);
end
