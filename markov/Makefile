# eat any lines with my name, empty lines, and replace tab characters
p4 changes -l -u bryce.kelleher | sed -e '/bryce.kelleher\|import/d' -e '/^\s*$/d' -e 's/^\t*//'
