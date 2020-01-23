#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

String thirdLevelMap[H] = {

    "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ",
    "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ",
    "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ",
    "                                                     B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                     B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                                                  B  BBBBBBBBB                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          ",
    " 1                 1                              B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "BBBBBBBBBBBBBBBBBBBB                              B  B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ",
    "                   B                              BBBB          BBBB                                                                                                                                                                                            1                                   1                                                           BBBBBBBB                                                                                                            1                         1                                                                                                          ",
    "                   B                                                                                                                                                                           BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB               BBBB         BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                                                                                           BBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                          ",
    "         BBBBBBBBBBB                                                                                                                                                                                                              B                            B                                        B                                                                                                                                                                                                   B                                                                                                           ",
    "         B                                                             BBBBBBBB                                                                                                                                                   B                            B                                        B                                                                               BBBBBBBBBBBBBBBBB                                                                                                   B                                                                                                           ",
    "         B                                                                                                                                                                           BBBB                                         B                            B                                        B                                                                               B               B                                                                                                   B                                                                                                           ",
    "         B                                                                                                                                                                  BBBB                                                       BBBB                                                             B                                                                               B               B            BBBBBBBBBBBBBBBBBBBB               BBBBBBBB    BBBB                                    B                                                                                                           ",
    "         B                                                                                                                                                 BBBBBBBB                                                                                                                                                                                                                     B               B            B                  B                                                                     BB                                                                                                        ",
    "          BBBB                                               BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                                                                                                                                                         BBBBBBBB                             B               B            B                  B                                                                      B                                                                                                        ",
    "             B1                          1                   B                                          BB1                                           1                                                                                                                                                                                                                             BBBBB               B            BBBBBBBBBBBBBBBBBBBB                                                                      BB                                                                                                       ",
    "             BBBBBBBBBBBBBBBBBBBBBBBBBBBBB        BBBBBBBBBBBB                                          BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                                                                                                                                                 B               B                                                                                                    B  B                                                                                                       ",
    "                                         B        B                                                                                                                                                                                                                                                                                                                                     B               B                                                                                                    B  B                                                                                                       ",
    "                                         B        B                                                                                                                                                                                               BB                                                                                                                                    B               B                                     BBBB                                                           B  B                                                                                                       ",
    "                                         B        B  B                                                                                                                                                                                            BB                                                                BBBBBBBBBBBBBBBBBBBBBBBBB                                           B               B                                     B  B                                                           B  BBB                                                                                                     ",
    "                                         B        B  B                                                                                                                                                                                           BBB                                                                B                       B                                           B               B                                     B  B                                                           B    B                                                                                                     ",
    "                                         B        B  B                                                                                                                                                                                          BBBB                                                                B                       B                                           BBBBBBBBBBBBBBBBB                                     BBBB                                                           B    BBB                                                                                                   ",
    "                                         BBB      B  B                                                                                                                                                                                         BBBBBB                                                               B                       B                                                                                                                                                                B      B                                                                                                   ",
    "BBBBBBBBBBBBBBBBBBBBBB                BBBB        B  B                                                                                                                                                                                         BBBBBB                                              BBBBBBBB         B                       B                                                                                                                                                                                                                                                                           ",
    "                     BBBB            BB           B  B                                                                                                                                 BBBBBBBB                                                BBBBBBBB                                                             B                       B                                                                                                        BB                                                                 BBBBBBBB                                                                                        ",
    "                        BBBBBBBBBBBBB             B  B                                                                                                                                                                              BBBB                                                                            B                       B                                                                                                        BB                                                                                                                                                                 ",
    "                                                BBB  B                                                                                                                                                              BBBBBBBB                                                    BBBBBBBB                            BBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                      BBBB                                                                                                                                                                 ",
    "                                                  B  B                       1                                                1                                                                                                                                                                                                                                                                                                                   B  B                                                                                                                                                                 ",
    "                                                  B  B                       BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                                                                                                                                                                                                                                  BBB  B                                                                                                                                                                 ",
    "                                                  B  B                       B                    B                        B  B                                                                                                                                                                                                                                                                                                                  B    B                                                                                                                                                                 ",
    "                                              BBBBB  B                       B                    B                        B  B                                                                                                                                                                                                                                                                                                                BBB    B                                     BBBB                                        BBBB                                                                            ",
    "                                                  B  B               BBBBBBBBB                    B                        B  B                                                                                                                                                                                                                                       BBBB                                                                     B      B                                     B  B                                        B  B                                                                            ",
    "                                                  B  B                       B                    B                        B  B                                                                        BBBB                                                         BBBB                                                                                              B  B                                                                   BBB      B                                     B  B                                        B  B                                                                            ",
    "                                                  B  BBB                     B                    B                        B  B                                                                        B  B                                                         BB B                                 BBBB                                                         B  B                                                                   B        B                                     B  B                                        B  B                                                                            ",
    "BBBBBBBBBBBBBBBBBBBBBB                            B  B                       B                    B                        B  B                                                                    BB  B  B                                                     BBBBB  B                                 B  B                                                         B  B                                                                 BBB        B                                     B  B                                        B  B                                                                            ",
    "                     B                            BBBB                       BBBBBBBBBBBBBBBBBBBBBB                        B  B                                                                    BB  B  B                                                     B      B                                 B  B                                                         B  B                                                                 B          B                                     B  B                                        B  B                                                                            ",
    "                     BBBBBBBBBBBBBBBBB                                                                                     BBBB                                                                  BBBBBBB  BBB                                               BBBBB      B                                 B  B                                                    BBBB B  B                                                               BBB          B                         BBBBBBBBBBBBB  BBB                                  BBBBB  BBB                                                                          ",
    "                                    BB                                                                                                                                                           B        BBB                                               B          B                                 B  B    1                                  1            BBBB B  B                                                               B            B                                 B        B                                  B        B                                                                           ",
    "                                     B                        BBBBBBBB                                                                                                     BBBBBBBBBBBBBBBBBBBBBBB           BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB        BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB        BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "                                     B                                                                                                                                     B                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
    "                                     BBBBBBB                                                                                                                               B                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
    "                                           B                                                                                                                               B                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
    "                                           BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                                                                                                                                                                                                                                                                                                                                                                                                                            ",
    "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ",
};