#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
* Caminho para o css
*/
#define CSS_PATH							"http://localhost/css/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n<html>\n")

/**
\brief Macro para começar o css
*/
#define COMECAR_CSS							printf("<link rel=%s type=%s href=%s%s>\n", "stylesheet", "text/css", CSS_PATH, "button.css")

/**
\brief Macro para começar o head
*/
#define COMECAR_HEAD						printf("<head>\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d>\n", tamx, tamy)
/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")

/**
\brief Macro para fechar um head
*/
#define FECHAR_HEAD							printf("</head>\n")


/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s%s />\n", \
												ESCALA * X, ESCALA * Y, ESCALA, ESCALA, IMAGE_PATH, FICHEIRO)

/**
\brief Macro para criar uma imagem com manipulação da escala
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM2(X, Y, TAMX, TAMY, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s%s />\n", \
												X, Y, TAMX, TAMY, IMAGE_PATH, FICHEIRO)

/**
\brief Macro para criar um quadrado
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COR)			printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, COR)

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")

/**
\brief Macro para fechar o html
*/
#define FECHAR_HTML							printf("</html>\n")

#define ABRIR_FORM(link)                    printf("<form action=%s>\n", link)

#define FECHAR_FORM                         printf("</form>\n")

#define ABRIR_DIV                           printf("<div>\n")

#define FECHAR_DIV                          printf("</div>\n")

#define ABRIR_LABEL                         printf("<label>\n")

#define FECHAR_LABEL                        printf("</label>\n")

#define SUBMIT_BOX(txt, user, size)         printf("%s: <input name=\"%s\" size=\"%d\">\n", txt, user, size)

#define SUBMIT_BUTTON(type, value)          printf("<input type=\"%s\" value=\"%s\">\n", type, value)

#endif