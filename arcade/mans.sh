for PLATFORM in dosmanuals snesmanuals genesismanuals n64manuals
do
    COUNT=$(ls $PLATFORM | wc -l)
    DIRCOUNT=$(ls -l $PLATFORM | grep drw | wc -l)
    FILECOUNT=$[COUNT-DIRCOUNT]
    COL_LEN=$[FILECOUNT/3 + 1]
    cat manhead > $PLATFORM.html
    echo "<div id=\"col3\">" >> $PLATFORM.html
    echo "    <ul class=\"hbdatalinks\">" >> $PLATFORM.html
    X=0
    for OBJECT in $(ls $PLATFORM)
    do
        if [ -f $PLATFORM/$OBJECT ]
        then
            BASE=${OBJECT%.*}
            echo "        <li><a href=\"$PLATFORM/$OBJECT\">$BASE</a></li>" >> $PLATFORM.html
            X=$[X + 1]
            if [ $X = $COL_LEN ] 
            then
                echo "    </ul>" >> $PLATFORM.html
                echo "</div>" >> $PLATFORM.html
                echo "<div id=\"col3\">" >> $PLATFORM.html
                echo "    <ul class=\"hbdatalinks\">" >> $PLATFORM.html
                X=0
            fi
        fi
    done
    echo "    </ul>" >> $PLATFORM.html
    echo "</div>" >> $PLATFORM.html
    echo "</body></html>" >> $PLATFORM.html

done
