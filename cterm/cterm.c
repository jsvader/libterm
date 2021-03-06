#include <stdio.h>
#include <errno.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libterm.h>
#include <sys/select.h>

#define WIDTH    80
#define HEIGHT    25

WINDOW *wnd;
int cursor_x = -1;
int cursor_y = -1;

void slog(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    va_end(args);
}

void update(term_t handle, int x, int y, int width, int height)
{
    int i, j;
    const wchar_t *const *grid = term_get_grid(handle);


    wattroff( wnd, A_REVERSE );
    for( i = 0; i < HEIGHT; i ++ ) {
        for( j = 0; j < WIDTH; j ++ ) {
            wmove( wnd, i, j );
            wdelch( wnd );
            if( j == cursor_x && i == cursor_y ) {
                wattron( wnd, A_REVERSE );
                winsch( wnd, grid[i][j] );
                wattroff( wnd, A_REVERSE );
            } else {
                winsch( wnd, grid[i][j] );
            }
        }
    }
    wrefresh(wnd);
}

void cursor(term_t handle, int old_x, int old_y, int new_x, int new_y)
{
    cursor_x = new_x;
    cursor_y = new_y;

    update( handle, old_x, old_y, 1, 1 );
    update( handle, new_x, new_y, 1, 1 );
}

int main(int argc, char *argv[])
{
    int length;
    fd_set rfds;
    int file_handle;
    int stdin_handle;
    term_t handle;
    WINDOW *screen;

    screen = initscr();

    wnd = newwin(HEIGHT, WIDTH, 0, 0);
    wclear( wnd );

    if( !term_create(&handle) ) {
        fprintf( stderr, "Failed to create a terminal handle (%s)\n", strerror( errno ) );
        goto done;
    }
    if( !term_set_shell(handle, "/bin/bash") ) {
        fprintf( stderr, "Failed to set shell (%s)\n", strerror( errno ) );
        goto done;
    }
    if( !term_begin(handle, WIDTH, HEIGHT, 0) ) {
        fprintf( stderr, "Failed to initialize terminal (%s)\n", strerror( errno ) );
        goto done;
    }

    term_register_update(handle, update);
    term_register_cursor(handle, cursor);
    stdin_handle = fileno(stdin);
    file_handle = term_get_file_descriptor(handle);

    while( 1 ) {
        FD_ZERO(&rfds);
        FD_SET(file_handle, &rfds);
        FD_SET(stdin_handle, &rfds);

        select( file_handle+1, &rfds, NULL, NULL, NULL );

        if(FD_ISSET(file_handle, &rfds)) {
            if( !term_process_child(handle) ) {
                break;
            }
        } else if(FD_ISSET(stdin_handle, &rfds)) {
            char buf[100];
            length = read(stdin_handle, buf, 100);
            term_send_data(handle, buf, length);
        }

    }

done:

    term_free( handle );

    delwin( wnd );
    delwin( screen );

    endwin( );

    return 0;
}
