#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct E {
    int start;
    int stop;
    char comando;
    int sizeUndo;
    int sizeRedo;
    char** linesUndo;
    char** linesRedo;
    int editorVuoto;
} Element;

char* readLine();
void readCommand(int* start, int* stop, char* comando);
void eseguiChange(int start, int stop, char*** array, int* ptLast, int* ptSize, Element** arrayUR, int* ptLastUR, int* ptSizeUR, int* ptCurrUR);
void eseguiPrint(int start, int stop, char** array, int last);
void setNewElement(int start, int stop, char comando, int sizeUndo, int sizeRedo, Element* arrayUR, int pos, int editorVuoto);
void eseguiDelete(int start, int stop, char*** array, int* ptLast, int* ptSize, Element** arrayUR, int* ptLastUR, int* ptSizeUR, int* ptCurrUR);
int eseguiUndo(char*** array, int* ptLast, int* ptSize, Element* arrayUR, int* ptCurrUR, int* ptLastUR, int numUndo, int* ptNumUndoDone, int numOp);
int eseguiRedo(char*** array, int* ptLast, int* ptSize, Element* arrayUR, int* ptCurrUR, int* ptLastUR, int numRedo, int* ptNumUndoDone, int numOp);
void eseguiChangeRedo(int start, int stop, char*** array, int* ptLast, int* ptSize, char** input, int numStringhe);

int moltiplicatore=10;

int main() {

    char** righeTesto=NULL;
    Element* UndoRedoArr=NULL;
    int sizeURArr=0;
    int currURArr=0;
    int lastURArr=-1;

    char* l=(char*)malloc(sizeof(char)*1025);

    int size=0;
    int last=-1;
    int start=0, stop=0;
    int numOp=0;
    char comando='q';

    int numUndoDone=0;
    int currNumUR=0;

    int quantities=0;

    do{
        readCommand(&start, &stop, &comando);
        if(comando=='c')
        {
            if(currNumUR>0)
            {
                quantities=eseguiUndo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, currNumUR, &numUndoDone, numOp);
                numOp=numOp-quantities;
                numUndoDone=0;
            }
            else if(currNumUR<0)
            {
                quantities=eseguiRedo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, -currNumUR, &numUndoDone, numOp);
                numOp=numOp+quantities;
                numUndoDone=0;
            }
            if(currURArr<=lastURArr)
            {
                numUndoDone=0;
                if(currURArr>=0) {
                    lastURArr = currURArr - 1;
                } else
                    {
                        lastURArr=-1;
                    }
                currURArr=lastURArr+1;
            }
            currNumUR=0;
            eseguiChange(start, stop, &righeTesto, &last, &size, &UndoRedoArr, &lastURArr, &sizeURArr, &currURArr);
            numOp=numOp+1;
        }
        else if(comando=='p')
        {
            if(currNumUR>0)
            {
                quantities=eseguiUndo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, currNumUR, &numUndoDone, numOp);
                numOp=numOp-quantities;
                currNumUR=0;
            }
            else if(currNumUR<0)
            {
                quantities=eseguiRedo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, -currNumUR, &numUndoDone, numOp);
                numOp=numOp+quantities;
                currNumUR=0;
            }
            eseguiPrint(start, stop, righeTesto, last);
        }
        else if(comando=='d')
        {
            if(currNumUR>0)
            {
                quantities=eseguiUndo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, currNumUR, &numUndoDone, numOp);
                numOp=numOp-quantities;
                numUndoDone=0;
            }
            else if(currNumUR<0)
            {
                quantities=eseguiRedo(&righeTesto, &last, &size, UndoRedoArr, &currURArr, &lastURArr, -currNumUR, &numUndoDone, numOp);
                numOp=numOp+quantities;
                numUndoDone=0;
            }
            if(currURArr<=lastURArr)
            {
                numUndoDone=0;
                if(currURArr>=0) {
                    lastURArr = currURArr - 1;
                } else
                {
                    lastURArr=-1;
                }
                currURArr=lastURArr+1;
            }
            currNumUR=0;
            eseguiDelete(start, stop, &righeTesto, &last, &size, &UndoRedoArr, &lastURArr, &sizeURArr, &currURArr);
            numOp=numOp+1;
        }
        else if(comando=='u')
        {
            if(currNumUR+start<=numOp)
            {
                currNumUR=currNumUR+start;
            } else
            {
                currNumUR=numOp;
            }
        }
        else if(comando=='r')
        {
            if(numUndoDone==0)
            {
                if(currNumUR-start>0)
                {
                    currNumUR=currNumUR-start;
                } else
                {
                    currNumUR=0;
                }
            } else
            {
                if(-(currNumUR-start)<=numUndoDone)
                {
                    currNumUR=currNumUR-start;
                } else
                {
                    currNumUR=-numUndoDone;
                }
            }
        }
    } while(comando!='q');
    return 0;
}

char* readLine()
{
    char s[1025];
    char* final;
    int posFinale;

    final=fgets(s, 1025, stdin);
    posFinale=strlen(s)-1;
    s[posFinale] = '\0';

    final = (char*)malloc((posFinale + 1) * sizeof(char));

    if(final!=NULL)
    {
        final=strcpy(final, s);
        return final;
    } else {
        return NULL;
    }
}

void readCommand(int* start, int* stop, char* comando)
{
    char c;
    char numero[11];
    int pos=0;

    do{
        c=getchar_unlocked();
        if(c!=',' && c!='q' && c!='u' && c!='r')
        {
            numero[pos]=c;
            pos++;
        } else
        {
            numero[pos]='\0';
        }
    } while(c!=',' && c!='q' && c!='u' && c!='r');

    if(c!='q')
    {
        *(start)=atoi(numero);
        if(c=='u' || c=='r')
        {
            *(comando)=c;
        } else
        {
            pos=0;
            do{
                c=getchar_unlocked();
                if(c!='c' && c!='d' && c!='p')
                {
                    numero[pos]=c;
                    pos++;
                }
                else
                {
                    numero[pos]='\0';
                }
            } while(c!='c' && c!='d' && c!='p');
            *(stop)=atoi(numero);
            *(comando)=c;
        }
        c=getchar_unlocked();
    } else
    {
        *(comando)=c;
    }
}

void eseguiChange(int start, int stop, char*** array, int* ptLast, int* ptSize, Element** arrayUR, int* ptLastUR, int* ptSizeUR, int* ptCurrUR)
{
    int last=*(ptLast);
    int size=*(ptSize);
    char** newArray=*(array);
    char* read;
    int i=start-1;

    int dimArrUndo=0;
    int dimArrRedo=stop-start+1;
    int pos=0;
    int posRedo=0;

    Element* newArrayUR=*(arrayUR);
    int lastUR=*(ptLastUR);
    int sizeUR=*(ptSizeUR);

    if(start-1>last)
    {
        dimArrUndo=0;
    }
    else if(stop-1<=last)
    {
        dimArrUndo=stop-start+1;
    } else
    {
        dimArrUndo=last-start+2;
    }

    if(last==-1 && size==0)
    {
        newArray=(char**)malloc((size+(stop-start+1)*moltiplicatore)*sizeof(char*));
        *(ptSize)=size+(stop-start+1)*moltiplicatore;
    }
    else if(stop>=size)
    {
        newArray=(char**)realloc(newArray, (size+(stop-start+1)*moltiplicatore)*sizeof(char*));
        *(ptSize)=size+(stop-start+1)*moltiplicatore;
    }

    lastUR=lastUR+1;

    if(newArrayUR==NULL)
    {
        newArrayUR=(Element*)malloc(moltiplicatore*sizeof(Element));
        *(ptSizeUR)=moltiplicatore;
    }
    else if(lastUR>=sizeUR)
    {
        newArrayUR=(Element*)realloc(newArrayUR, (sizeUR+moltiplicatore)*sizeof(Element));
        *(ptSizeUR)=sizeUR+moltiplicatore;
    }

    if(last==-1) {
        setNewElement(start, stop, 'c', dimArrUndo, dimArrRedo, newArrayUR, lastUR, 1);
    } else
    {
        setNewElement(start, stop, 'c', dimArrUndo, dimArrRedo, newArrayUR, lastUR, 0);
    }

    for(i=start-1; i<=stop-1; i++)
    {
        read=readLine();
        if(i<=last)
        {
            newArrayUR[lastUR].linesUndo[pos]=newArray[i];
            pos++;
        }
        newArrayUR[lastUR].linesRedo[posRedo]=read;
        newArray[i]=read;
        posRedo++;
    }

    read=readLine();

    if(i-1>*(ptLast)) {
        *(ptLast) = i - 1;
    }

    *(array)=newArray;
    *(ptLastUR)=lastUR;
    *(arrayUR)=newArrayUR;
    *(ptCurrUR)=lastUR+1;
}

void eseguiPrint(int start, int stop, char** array, int last)
{
    for(int i=start-1; i<stop; i++)
    {
        if(i<=last && i>=0 && last!=-1)
        {
            printf("%s\n", array[i]);
        } else
        {
            printf(".\n");
        }
    }
}

void setNewElement(int start, int stop, char comando, int sizeUndo, int sizeRedo, Element* arrayUR, int pos, int editorVuoto)
{
    arrayUR[pos].start=start;
    arrayUR[pos].stop=stop;
    arrayUR[pos].comando=comando;
    arrayUR[pos].sizeUndo=sizeUndo;
    arrayUR[pos].sizeRedo=sizeRedo;
    arrayUR[pos].editorVuoto=editorVuoto;
    if(sizeUndo>0)
    {
        arrayUR[pos].linesUndo=(char**)malloc(sizeUndo*sizeof(char*));
    } else
    {
        arrayUR[pos].linesUndo=NULL;
    }
    if(sizeRedo>0)
    {
        arrayUR[pos].linesRedo=(char**)malloc(sizeRedo*sizeof(char*));
    } else
    {
        arrayUR[pos].linesRedo=NULL;
    }
}

void eseguiDelete(int start, int stop, char*** array, int* ptLast, int* ptSize, Element** arrayUR, int* ptLastUR, int* ptSizeUR, int* ptCurrUR)
{
    int last=*(ptLast);
    char** newArray=*(array);
    int j=stop;
    int size=*(ptSize);

    int dimArrUndo=0;
    int pos=0;
    Element* newArrayUR=*(arrayUR);
    int lastUR=*(ptLastUR);
    int sizeUR=*(ptSizeUR);

    if((start==0 && stop==0)||(start-1>last))
    {
        dimArrUndo=0;
    }
    else if(start==0)
    {
        if(stop-1<=last)
        {
            dimArrUndo=stop;
        } else
        {
            dimArrUndo=last+1;
        }
    } else
    {
        if(stop-1<=last)
        {
            dimArrUndo=stop-start+1;
        } else
        {
            dimArrUndo=last-start+2;
        }
    }

    lastUR=lastUR+1;

    if(newArrayUR==NULL)
    {
        newArrayUR=(Element*)malloc(moltiplicatore*sizeof(Element));
        *(ptSizeUR)=moltiplicatore;
    }
    else if(lastUR>=sizeUR)
    {
        newArrayUR=(Element*)realloc(newArrayUR, (sizeUR+moltiplicatore)*sizeof(Element));
        *(ptSizeUR)=sizeUR+moltiplicatore;
    }

    if(start==0)
    {
        start=1;
    }

    if(last==-1)
    {
        setNewElement(start, stop, 'd', last+1, last-dimArrUndo+1, newArrayUR, lastUR, 1);
    } else{
        setNewElement(start, stop, 'd', last+1, last-dimArrUndo+1, newArrayUR, lastUR, 0);
    }


    if(dimArrUndo==0)
    {
        for(int i=0; i<=last; i++)
        {
            newArrayUR[lastUR].linesUndo[i]=newArray[i];
        }
        newArrayUR[lastUR].linesRedo=newArrayUR[lastUR].linesUndo;
    } else
    {
        for(int i=0; i<=last; i++)
        {
            newArrayUR[lastUR].linesUndo[i]=newArray[i];
            if(i<start-1 || i>stop-1){
                newArrayUR[lastUR].linesRedo[pos]=newArray[i];
                pos++;
            }
            if(i>=start-1)
            {
                if(j<=last) {
                    newArray[i] = newArray[j];
                    j++;
                }
            }
        }
    }

    *(array)=newArray;
    *(ptLast)=last-dimArrUndo;
    *(ptLastUR)=lastUR;
    *(arrayUR)=newArrayUR;
    *(ptCurrUR)=lastUR+1;
}

int eseguiUndo(char*** array, int* ptLast, int* ptSize, Element* arrayUR, int* ptCurrUR, int* ptLastUR, int numUndo, int* ptNumUndoDone, int numOp)
{
    int last=*(ptLast);
    int size=*(ptSize);
    char** newArray=*(array);

    int cont=numUndo;
    int i;
    int numUndoDone=0;

    Element* newArrayUR=arrayUR;
    int currUR=*(ptCurrUR);
    int lastUR=*(ptLastUR);
    int pos=currUR-numUndo;

    int contChange;
    int trovato=0;

    if(numUndo>=numOp)
    {
        currUR=0;
        last=-1;
        numUndoDone=numOp;
    }
    else if(pos>=0 && newArrayUR[pos].editorVuoto==1)
    {
        currUR=pos;
        last=-1;
        numUndoDone=numUndo;
    }
    else if(pos>=0 && newArrayUR[pos].comando=='d')
    {
        for(int k=0; k<newArrayUR[pos].sizeUndo; k++)
        {
            newArray[k]=newArrayUR[pos].linesUndo[k];
        }

        currUR=pos;
        last=newArrayUR[pos].sizeUndo-1;
        numUndoDone=numUndo;
    }
    else {

        contChange=pos+1;
        cont=1;
        while(contChange<currUR && trovato==0)
        {
            if(newArrayUR[contChange].comando=='d')
            {
                trovato=1;
            } else
            {
                contChange=contChange+1;
                cont=cont+1;
            }
        }

        if(trovato==1)
        {
            if(newArrayUR[contChange].editorVuoto==1)
            {
                last=-1;
            } else {
                if ((last == -1 && newArrayUR[contChange].sizeUndo > size) || (last + 1 + newArrayUR[contChange].sizeUndo > size))
                {
                    newArray = (char **) realloc(newArray,(size + (newArrayUR[contChange].sizeUndo) * moltiplicatore) * sizeof(char *));
                    size = size + (newArrayUR[contChange].sizeUndo) * moltiplicatore;
                }

                for (int k = 0; k < newArrayUR[contChange].sizeUndo; k++) {
                    newArray[k] = newArrayUR[contChange].linesUndo[k];
                }

                last = newArrayUR[contChange].sizeUndo - 1;
            }
            numUndoDone=currUR-contChange;
            currUR = contChange;
        } else
        {
            cont=numUndo;
        }

        currUR = currUR - 1;
        while (currUR>=0 && cont > 0) {
            i = 0;
            pos = 0;
            if (newArrayUR[currUR].comando == 'c') {
                if (newArrayUR[currUR].sizeUndo == 0)
                {
                    last = newArrayUR[currUR].start - 2;
                } else {
                    i = newArrayUR[currUR].start - 1;
                    while (pos < newArrayUR[currUR].sizeUndo)
                    {
                        newArray[i] = newArrayUR[currUR].linesUndo[pos];
                        pos++;
                        i++;
                    }
                    if (i - 1 < last && newArrayUR[currUR].stop > last) {
                        last = i - 1;
                    }
                }
            } else {
                if (newArrayUR[currUR].sizeUndo != 0) {
                    if ((last == -1 && newArrayUR[currUR].sizeUndo > size) || (last + 1 + newArrayUR[currUR].sizeUndo > size)) //CORREZIONE
                    {
                        newArray = (char **) realloc(newArray, (size + (newArrayUR[currUR].sizeUndo) * moltiplicatore) * sizeof(char *));
                        size = size + (newArrayUR[currUR].sizeUndo) * moltiplicatore;
                    }
                    for(int k=0; k<newArrayUR[currUR].sizeUndo; k++)
                    {
                        newArray[k]=newArrayUR[currUR].linesUndo[k];
                    }

                    last=newArrayUR[currUR].sizeUndo-1;
                }
            }
            cont--;
            if(cont>0) {
                currUR = currUR - 1;
            }
            if (currUR<0){
                cont=0;
                currUR=0;
            }
            numUndoDone = numUndoDone + 1;
        }
    }
    *(ptLast)=last;
    *(ptSize)=size;
    *(ptNumUndoDone)=*(ptNumUndoDone)+numUndoDone;
    *(ptCurrUR)=currUR;
    *(array)=newArray;
    return numUndoDone;
}

int eseguiRedo(char*** array, int* ptLast, int* ptSize, Element* arrayUR, int* ptCurrUR, int* ptLastUR, int numRedo, int* ptNumUndoDone, int numOp)
{
    int last=*(ptLast);
    int size=*(ptSize);
    char** newArray=*(array);

    int cont=numRedo;
    int numRedoDone=0;

    Element* newArrayUR=arrayUR;
    int currUR=*(ptCurrUR);
    int lastUR=*(ptLastUR);

    int pos=currUR+numRedo;

    int contChange;
    int trovato=0;

    if(pos>=0 && pos<=lastUR && newArrayUR[pos].editorVuoto==1)
    {
        currUR=pos;
        last=-1;
        numRedoDone=numRedo;
    }
    else if(pos>=0 && pos<=lastUR && newArrayUR[pos].comando=='d')
    {
        for(int k=0; k<newArrayUR[pos].sizeUndo; k++)
        {
            newArray[k]=newArrayUR[pos].linesUndo[k];
        }

        currUR=pos;
        last=newArrayUR[pos].sizeUndo-1;
        numRedoDone=numRedo;
    } else {
        if(currUR==-1)
        {
            currUR=0;
        }

        contChange=pos-1;
        cont=1;
        while(contChange>currUR && trovato==0)
        {
            if(newArrayUR[contChange].comando=='d')
            {
                trovato=1;
            } else
            {
                contChange=contChange-1;
                cont=cont+1;
            }
        }

        if(trovato==1)
        {
            if(newArrayUR[contChange].editorVuoto==1)
            {
                last=-1;
            } else {
                if ((last == -1 && newArrayUR[contChange].sizeUndo > size) || (last + 1 + newArrayUR[contChange].sizeUndo > size))
                {
                    newArray = (char **) realloc(newArray,(size + (newArrayUR[contChange].sizeUndo) * moltiplicatore) * sizeof(char *));
                    size = size + (newArrayUR[contChange].sizeUndo) * moltiplicatore;
                }

                for (int k = 0; k < newArrayUR[contChange].sizeRedo; k++) {
                    newArray[k] = newArrayUR[contChange].linesRedo[k];
                }

                last = newArrayUR[contChange].sizeRedo - 1;
            }
            numRedoDone=contChange-currUR+1;
            currUR = contChange+1;
            cont=cont-1;
        } else
        {
            cont=numRedo;
        }

        while (currUR<=lastUR && cont > 0) {
            if (newArrayUR[currUR].comando == 'c') {
                eseguiChangeRedo(newArrayUR[currUR].start, newArrayUR[currUR].stop, &newArray, &last, &size, newArrayUR[currUR].linesRedo, newArrayUR[currUR].sizeRedo);
            } else {
                for(int k=0; k<newArrayUR[currUR].sizeRedo; k++)
                {
                    newArray[k]=newArrayUR[currUR].linesRedo[k];
                }

                last=newArrayUR[currUR].sizeRedo-1;
            }
            cont--;
            currUR = currUR + 1;
            if (currUR>lastUR){
                cont=0;
            }

            numRedoDone = numRedoDone + 1;
        }
    }
    *(ptLast)=last;
    *(ptSize)=size;
    *(ptNumUndoDone)=*(ptNumUndoDone)-numRedoDone;
    *(ptCurrUR)=currUR;
    *(array)=newArray;
    return numRedoDone;
}

void eseguiChangeRedo(int start, int stop, char*** array, int* ptLast, int* ptSize, char** input, int numStringhe)
{
    int last=*(ptLast);
    int size=*(ptSize);
    char** newArray=*(array);
    char* read;
    int i=start-1;
    int pos=0;

    if(last==-1 && size==0)
    {
        newArray=(char**)malloc((size+(stop-start+1)*moltiplicatore)*sizeof(char*));
        *(ptSize)=size+(stop-start+1)*moltiplicatore;
    }
    else if(stop>=size)
    {
        newArray=(char**)realloc(newArray, (size+(stop-start+1)*moltiplicatore)*sizeof(char*));
        *(ptSize)=size+(stop-start+1)*moltiplicatore;
    }

    for(int j=0; j<numStringhe; j++, i++, pos++)
    {
        read=input[pos];
        newArray[i]=read;
    }

    if(i-1>*(ptLast)) {
        *(ptLast) = i - 1;
    }
    *(array)=newArray;
}