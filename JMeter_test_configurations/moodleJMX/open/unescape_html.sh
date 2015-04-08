cat login.csv | perl -MHTML::Entities -le 'while(<>) {print decode_entities($_);}' > file.html
