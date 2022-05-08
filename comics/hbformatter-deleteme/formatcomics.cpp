#include <iostream>
#include <fstream>
//#include <sstream>
#include <stdio.h>
//#include <vector>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h
#include <stdio.h>
#include <string.h>
int getToken(char *, char *);
void removeDelimiter(char);
void addDelimiter(char);
void setLeadTrimWhite(void);
void setDelimitWhite(void);
int skipit(char);
const int STRONG_SPACE = 1;
int strong_flags = 0;
#define COLLECTION 100
int bagpos;

int main (int argc, char*argv[])
{
    FILE *fin, *fout, *js;
    char pagetitle[9999];
    char line[9999 + 1];
    char series[9999];
    char number[9999];    
    char issueTitle[9555];
    char tempTitle[9555];
    int  titleOut;
    int  titleIn;
    char comment[9555];
    char reprint1[9555];
    char reprint2[9555];
    char reprints;
    char tmp[9999 + 1];
    char coloncomment[9999];
    char celltype[999];
    bool hbvolume;
    int c; 
    int l;
    int color = 1;
    char hexcolor[9];
    char yellow[8] = "#f9f9a5";
    char purple[8] = "#decaff";
    char justbecause[8] = "#ddaaaa";
    strcpy(hexcolor, yellow);

//    setLeadTrimWhite();
    setDelimitWhite();
    addDelimiter('@'); //  reprint
    addDelimiter('*'); //  comment
    addDelimiter('"'); //  title  
    addDelimiter('^'); //  bag position
    addDelimiter('}'); //  hb binding/volume flag

    for(int x = 1; x < argc; x++){
        if(argv[x][0] == '_')
            continue;
        fin = fopen(argv[x], "r");
        if(fin != NULL){

            char outName[555];
            int c = strlen(argv[x]);
            //  get file name from full path-qualified file name
            while(argv[x][c] != '/'  &&  c != 0){
                c--;
            }
            if(argv[x][c] == '/')
                c++;
            int cc = 0;
            while(c < strlen(argv[x])){
                outName[cc++] = argv[x][c++];
            }
            outName[cc] = 0;
            strcpy(pagetitle, outName);
            strcat(outName, ".html");
          
            fout = fopen(outName, "w");
            int maps = 0;
            char abb[444][20];
            char title[444][100];
            char word[9999];
            
            fputs("<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"../hb.css\"/>\n", fout);
            fputs("<link href=\"comics.ico\" rel=\"icon\" type=\"image/bmp\">\n", fout);
            //  insert javascript 
            js = fopen("comics.js", "r");
            while(fgets(line, 9999, js) != NULL){
                fputs(line, fout);
            }
            fclose(js);
            //  body of HTML
            fprintf(fout, "<title>%s</title></head>", pagetitle);
            fputs("<body>", fout);
            fputs("<center>", fout);
            fputs("<table cellpadding=\"12\" border=\"2\" id=\"issuetable\">", fout);
            fputs("<tr align=\"center\" > <td><b>Series</b></td> <td><b>Issue</b></td> <td><b>Comments</b></td> <td><b>Title</b></td>\n <td><b><input type=\"button\" value=\"Reprint\" onClick=\"swapReprint()\"></b></td>\n <td><b>Reprint</b></td>   </tr>", fout);
            while(fgets(line, 9999, fin) != NULL){
                if(line[0] != ' ' && line[0] != '-' && line[0] != 0 
                && line[0] != 10 && line[0] != 13){
                //  Line contains data
                    bagpos = 0;
                    hbvolume = false;
                    int n;
                    if(line[0] == '%'){
                        n = 6;
                    }
                    else{
                        n = 0;
                    }
                    if(line[n] == '#'){
                    //  This line is a map assignment
                        n += getToken(line + n, word); 
                        n += getToken(line + n, word);
                        strcpy(abb[maps], word);
                        n +=  getToken(line + n, word);  //  should be '='
                        removeDelimiter(' ');
                        n += getToken(line + n, word);  //  should be title
                        addDelimiter(' ');
                        strcpy(title[maps], word);
                        maps++;
                    }
                    else if(line[n] == ':'){
                        strcpy (coloncomment, line + n + 1);
                        fprintf(fout, "<tr bgcolor=\"#cecece\"><td colspan=\"6\">%s</td></tr>", coloncomment);
                    }
                    else{
                    //  translate and write this line to html file
                        n += getToken(line + n, word);
                        for(int i = 0; i < maps; i++){
                            if(!strcmp(word, abb[i])){
                                strcpy(word, title[i]);
                                break;
                            }
                        }
                        strcpy(series, word);
                        if(series[0] == '.') {
                            bagpos = COLLECTION;
                            //  more TBP subs will be done after
                            //  the line has been fully parsed
                            //  NO LONGER TRUE:
                            // want to be able to use quotes in the reprint 
                            // sections without them being interpreted as an
                            // issue/story title
                            /*removeDelimiter('"'); */
                        }
                        removeDelimiter(' ');
                        n += getToken(line + n, number);
                        //  Read optional comment, story title, and 2 reprints
                        strcpy(issueTitle, "&nbsp");
                        strcpy(comment,    "&nbsp");
                        strcpy(reprint1,   "&nbsp");
                        strcpy(reprint2,   "&nbsp");
                        reprints = 0;
                        //  Loop through entries in line
                        while(1){
                        /*  because ' ' should not be a delimiter before most 
                            calls to getToken(), most words will have trailing 
                            spaces and line[n] will most likely be on a 
                            non-space delimiter - the following code counts on 
                            it, so better refactor this to make sure it always 
                            works this way */
                        //  line[n] is now on a delimiter
                            if(*(line + n) == '@'){
                                //  read abbreviation:
                                addDelimiter(' ');
                                n++; //  move off of '@'
                                n += getToken(line + n, word);
                                //  attempt to expand abbreviation:
                                char reptitle[999];
                                strcpy(reptitle, word); // in case of no match  
                                for(int i = 0; i < maps; i++){
                                    if(!strcmp(word, abb[i])){
                                        strcpy(reptitle, title[i]);
                                        break;
                                    }
                                }
                                //  read reprint issue number, etc
                                removeDelimiter(' ');
                                n += getToken(line + n, word);
                                strcat(reptitle, " ");
                                strcat(reptitle, word);
                                if(reprints == 0)
                                    strcpy(reprint1, reptitle);
                                else
                                    strcpy(reprint2, reptitle);
                                reprints++;
                            }
                            else if(*(line + n) == '*'){
                                removeDelimiter(' ');
                                n++; //  move off of '*'
                                n += getToken(line + n, word);
                                strcpy(comment, word);
                            }
                            else if(*(line + n) == '}'){
                                n++; //  move off of flag character
                                hbvolume = true;
                                //  set up for next field:
                                while(line[n] == ' ')
                                    n++;
                            }
                            else if(*(line + n) == '^'){
                                n++; //  move off of '^'
                                bagpos = *(line + n);
                                n++; //  move off of bag position
                                //  set up for next field:
                                while(line[n] == ' ')
                                    n++;
                            }
                            else if(*(line + n) == '"'){
                                removeDelimiter(' ');
                                n++; //  move off of '"'
                                n += getToken(line + n, word);
                                issueTitle[0] = '"';

                                titleOut = 1;
                                titleIn  = 0;
                                while(word[titleIn] != 0){
                                    if(word[titleIn] == '|'){
                                        issueTitle[titleOut++] = '"';
                                        issueTitle[titleOut++] = '<';
                                        issueTitle[titleOut++] = 'b';
                                        issueTitle[titleOut++] = 'r';
                                        issueTitle[titleOut++] = '>';
                                        issueTitle[titleOut++] = '"';
                                    }
                                    else{
                                        issueTitle[titleOut] = word[titleIn];
                                        titleOut++;
                                    }
                                    titleIn++;
                                }
                                issueTitle[titleOut] = '"';
                                issueTitle[titleOut + 1] = 0;


                                // before I allowed for multiple titles:
                                /*strcpy(issueTitle + 1, word);
                                strcat(issueTitle, "\"");*/


                                n++; //  get off of terminating '"'
                                //  set up for next field:
                                while(line[n] == ' ')
                                    n++;
                            }
                            else  //  'n' is not on a delimiter, done with line
                                break;
                        }  //  End of looping through entries within the line
                        strcpy(celltype, "<td>");
                        if(series[0] == '.')
                        {
                            strcpy(celltype, "<td style='border-bottom:none;border-top:none'>");
                            if(!strcmp(series, ".0")){
                                strcpy(series, "&nbsp");
                                strcpy(number, "&nbsp");
                            }
                            else if(!strcmp(series, ".A")){
                                strcpy(celltype, "<td style='border-bottom:none'>");
                                strcpy(series, "&nbsp");
                                strcpy(number, "&nbsp");
                            }
                            else if(!strcmp(series, ".Z")){
                                strcpy(celltype, "<td style='border-top:none'>");
                                strcpy(series, "&nbsp");
                                strcpy(number, "&nbsp");
                            }
                            else{
                                strcpy(tmp, "<b>");
                                strcat(tmp, series + 1);
                                strcat(tmp, "</b>");
                                strcpy(series, tmp);
                                strcpy(number, "(tpb/gn)");
                            }
                            /*if(!strcmp(issueTitle, "&nbsp")){
                                //issueTitle[0] = '"';
                                //strcpy(issueTitle + 1, series + 1);
                                //strcat(issueTitle, "\"");
                                strcpy(issueTitle, series);
                            }*/
                            //strcpy(issueTitle, "&nbsp"); // now allowing story titles for TPBs
                            bagpos = COLLECTION;
                        }
                        switch (bagpos){
                            case '?':
                                fprintf(fout, "<tr bgcolor=\"%s\">", justbecause);
                                break;
                            case '9':
                                fprintf(fout, "<tr bgcolor=\"#bbffff\">");
                                break;
                            case 0:
                                fprintf(fout, "<tr bgcolor=\"#ececec\">");
                                break;
                            case COLLECTION: // tpb
                                fprintf(fout, "<tr bgcolor=\"#ffffff\">");
                                break;
                            case '1':
                                fprintf(fout, "<tr bgcolor=\"%s\">", hexcolor);
                                break;
                            case '2':
                                fprintf(fout, "<tr bgcolor=\"%s\">", hexcolor);
                                if(color == 1){
                                    color = 2;
                                    strcpy(hexcolor, purple);
                                }
                                else{
                                    color = 1;
                                    strcpy(hexcolor, yellow);
                                }
                                break;
                        };
                        if(hbvolume){
                            fprintf(fout, "<td bgcolor=\"#ddffdd\">%s</td>", series); 
                        }
                        else{
                            fprintf(fout, "%s%s</td>", celltype, series); 
                        }
                        fprintf(fout, "%s%s</td>", celltype, number); 
                        fprintf(fout, "%s%s</td>", celltype, comment); 
                        fprintf(fout, "%s%s</td>", celltype, issueTitle); 
                        fprintf(fout, "%s%s</td>", celltype, reprint1); 
                        fprintf(fout, "%s%s</td>", celltype, reprint2); 
                        fprintf(fout, "</tr>\n");
                        addDelimiter(' ');
                    }  //  end processing and writing line to html file
                    /*if (bagpos == COLLECTION){
                        addDelimiter('"'); 
                    }*/ //  no longer making exception that quotes in the reprint sections of TBPs are not treated as issue/story titles 
                } // end of line-contains-data (not-a-comment) block
            } //  bottom of file/line-reading loop
   
            fputs("</table>", fout);
            fputs("</center>", fout);
            fputs("</body>", fout);
            fputs("</html>", fout);
   
            fclose(fout);
            fclose(fin);
        }
    }
    
    return 0;
}

char delimit[555];
int stoppers;
#ifdef BLAH
//  this is new:
char trim[555];
int trims;

/////////////////////////////////////////////////////////////////////////////
void setLeadTrimWhite(void)
{
    trim[0] = ' ';
    trim[1] = '\t';
/*    trim[2] = '\n';
    trim[3] = 10;  
    trim[4] = 13;  
    trims = 5; */
    trims = 2; 
}
#endif
/////////////////////////////////////////////////////////////////////////////
void setDelimitWhite(void)
{
    delimit[0] = 0;
    delimit[1] = ' ';
    delimit[2] = '\t';
    delimit[3] = '\n';
    delimit[4] = 10;  
    delimit[5] = 13;  
    stoppers = 6;
}

/////////////////////////////////////////////////////////////////////////////
int getToken(char *string, char *token)
//  Fills "token" from "string" after discarding leading whitespace. 
//  Return # of chars read before getting to last char of token, including leading whitespce.
//  Return value  = position in string just to the right of last char of token.
{
    int max = 99999;  //  had probs making this an arg le default value

    int retval = 0;
    //  Move past leading delimiters
    while(skipit(*(string + retval))){
        retval++;
    }
    
    int tokenlen = 0;
    int s;
    while(tokenlen < max){
        // check if retval is on a delimiter
        for(s = 0; s < stoppers; s++){
            if(delimit[s] == string[retval]){
                break;
            }
        }
        if(s < stoppers){
            if(tokenlen == 0){

            }
            break;
        }

        token[tokenlen] = string[retval];
        tokenlen++;
        retval++;
    }
    token[tokenlen] = 0;
    return retval;
}

/////////////////////////////////////////////////////////////////////////////
int skipit(char c)
{
/*    int x;

    for(x = 0; x < stoppers; x++){
        if(delimit[x] == c)
            return 1;
    }*/

    if(c == ' ' || c == '\t')
        return 1;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
void addDelimiter(char c)
{
    int x;
    for(x = 0; x < stoppers; x++){
        if(delimit[x] == c){
            break;
        }
    }
    if(x == stoppers){
        delimit[stoppers++] = c;
    }
}

/////////////////////////////////////////////////////////////////////////////
void removeDelimiter(char c)
{
    int x;
    bool removing = false;
    for(x = 0; x < stoppers; x++){
        if(removing){
            delimit[x] = delimit[x + 1];
        }
        else{
            if(delimit[x] == c){
                delimit[x] = delimit[x + 1];
                removing = true;
            }
        }
    }
    if(removing){
        stoppers--;
    }
}

enum FSO_TYPE {none, file, dir, dot};
FSO_TYPE fsotype(string);
...
...
...
FSO_TYPE fsotype(string str)
{
    if(str == "." || str == "..")
        return dot;
    struct stat sb;
    if(stat(str.c_str(), &sb) == 0){
    //  the string does represent some kind of non-parent, non-self filesystem object
        if(S_ISDIR(sb.st_mode)){
            return dir;
        }
        else{
            return file;
        }
    }
    else{
        return none;
    }
}   
