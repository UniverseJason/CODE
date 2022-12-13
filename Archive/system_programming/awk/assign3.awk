BEGIN{
    # r=0 is user name *
    # r=1 is user totalfile *
    # r=2 is user hidden file *
    # r=3 is user dir *
    # r=4 is user other file *
    # r=5 is user storage *
    r=0
    c=0
    userList[r][c]=""
    
    newerT="00:00:00"
    newerD="0000/01/01"
    olderT="23:59:59"
    olderD="9999/12/31"
}

{
    # check if this is a new user
    add=1
    for(i=0; i<=c; i++)
    {
        if(userList[0][i]==$3) # old user need update the record
        {
            add=0
            break
        }
    }

    if(add == 1) # new user create new space and read new line
    {
        userList[0][c]=$3
        c++
    }
    
    # for dir
    if($1 ~ /^d[-rwx]{3}/)
    {
        dir++
        userList[3][i]+=1
    } 
    else if($1 ~ /^-[-rwx]{3}/) # for file
    {
        file++
        fileStorage+=$5
        userList[1][i]+=1
        userList[5][i]+=$5
        
        # for newer day
        currD=$6
        currT=$7
        if(currD > newerD)
        {
            newLine=$0
            newerT=currT
            newerD=currD
        }else if(currD == newerD && currT > newerT)
        {
            newLine=$0
            newerT=currT
            newerD=currD
        }
        
        # for older day
        if(currD < olderD)
        {
            oldLine=$0
            olderT=currT
            olderD=currD
        }else if(currD == olderD && currT > olderT)
        {
            oldLine=$0
            olderT=currT
            olderD=currD
        }
        
        # for hidden file
        if($NF ~ /^\..+\./)
        {
            hidfile++
            userList[2][i]+=1
        }
    }
    else if($1 ~ /^[^d-][-rwx]{3}/) # for other file
    {
        other++
        userList[4][i]+=1
    }
    

}

END{
    # Username: kaq447
    # Files:
    #             All: 2
    #          Hidden: 0
    #     Directories: 3
    #     Storage (B): 3092 bytes
    
    holder=""
    for(i=0; i<c; i++)
    {
        printf("\nUsername : %s\nFiles:\n", userList[0][i])
        if(userList[1][i]>0){printf("            All: %d\n", userList[1][i])}
        if(userList[2][i]>0){printf("         Hidden: %d\n", userList[2][i])}
        if(userList[3][i]>0){printf("    Directories: %d\n", userList[3][i])}
        if(userList[4][i]>0){printf("         Others: %d\n", userList[4][i])}
        if(userList[5][i]>0){printf("    Storage (B): %d\n", userList[5][i])}
    }
    
    printf("\n")
    printf("Newest file : \n\t%s\n", newLine)
    printf("Oldest file : \n\t%s\n", oldLine)
    
    printf("\n")
    printf("       Total user: %d\n",i)
    printf("      Total files:\n")
    printf("   (All / Hidden): (%d/%d)\n", file, hidfile)
    printf("Total directories: %d\n",dir)
    printf("     Total others: %d\n",other)
    printf("      Storage (B): %d\n",fileStorage)
}
