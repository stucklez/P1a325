void createClusters (points point[AMOUNT_OF_POINTS]) {
    int a, b, c, i; 

    for (i = 0; i < AMOUNT_OF_POINTS; i++) {
        if (strcmp(point[i].name, "Pickup1") == 0) {
            a = i; 
        }
        if (strcmp(point[i].name, "Pickup2") == 0) {
            b = i;
        }
        if (strcmp(point[i].name, "Pickup3") == 0) {
            c = i;
        }
        printf("%d %d %d\n", a,b,c);
    }

    for (i = 0; i < AMOUNT_OF_POINTS; i++) {

        if (lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)
        && lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
            point[i].status = 1;
        }
        if (lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
            point[i].status = 3;
        }
        if (lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)) {
            point[i].status = 2;

        
    }
    printf("%s %d\n",point[i].name, point[i].status);
}
}