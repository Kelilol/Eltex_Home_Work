//contact.c
#include "contact.h"
#include "string.h"

void printMenuEditContact() {
    printf("\nEdit Menu Contact:\n");
    printf("1. Edit first name\n");
    printf("2. Edit second name\n");
    printf("3. Edit jobPosition\n");
    printf("4. Edit workplace\n");
    printf("5. Edit phoneNumbers\n");
    printf("6. Edit socialLinks\n");
    printf("7. Edit email\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}


//PhoneNumbers
void printMenuEditPhoneNombers() {
    printf("\nEdit Menu Phone Nombers:\n");
    printf("1. Edit phone number\n");
    printf("2. Add phone number\n");
    printf("3. Delete phone number\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void addphoneNumbers(Contact *contact){
    if(contact->numphoneNumbers==MAX_PHONE_NUMBERS)
    {
        printf("\nPhone numbers is full:\n");
        return;
    } else
    {
        printf("Enter phone number: ");
        addPhoneNumber(&contact->phoneNumbers[contact->numphoneNumbers++]);
        printf("Contact added successfully\n");
    }
    
}

void editphoneNumber(Contact *contact){
    printf("View phone nombers");
    for(int i=0;i<contact->numphoneNumbers;i++)
    {
        printf("Phone %d: %s\n", i + 1, contact->phoneNumbers[i].phoneNumber);
    }
    int num=0;
    char input[2];
    printf("Enter num phone number for edit (1, 2...) : ");
    //scanf("%d", &num);
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    if (1>num&&num>(contact->numphoneNumbers+1)){
        printf("Incorrect input \n");
        return;
    }
    DeletePhoneNumber(&contact->phoneNumbers[num-1]);
    printf("Enter phone number for edit: ");
    addPhoneNumber(&contact->phoneNumbers[num-1]);

    printf("Contact updated successfully\n");

}

void deletephoneNumbers(Contact *contact){
    printf("View phone nombers: \n");
    for(int i=0;i<contact->numphoneNumbers;i++)
    {
        printf("Phone %d: %s\n", i + 1, contact->phoneNumbers[i].phoneNumber);
    }
    int num=0;
    char input[2];
    printf("Enter num phone number for edit (1, 2...) : ");
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    //scanf("%d", &num);
    if (1>num&&num>(contact->numphoneNumbers+1)){
        printf("Incorrect input \n");
        return;
    }
   
    DeletePhoneNumber(&contact->phoneNumbers[num-1]);
    if (num-1<contact->numphoneNumbers)
    {
        PhoneNumber PhoneNumber;
        for (int i = num-1;i<contact->numphoneNumbers;i++)
        {
            PhoneNumber=contact->phoneNumbers[i];
            contact->phoneNumbers[i]=contact->phoneNumbers[i+1];
            contact->phoneNumbers[i+1]=PhoneNumber;
        }
        DeletePhoneNumber(&contact->phoneNumbers[contact->numphoneNumbers]);
    }
    contact->numphoneNumbers--;
        printf("View phone nombers: \n");
    for(int i=0;i<contact->numphoneNumbers;i++)
    {
        printf("Phone %d: %s\n", i + 1, contact->phoneNumbers[i].phoneNumber);
    }
}

void editphoneNumbers(Contact *contact)
{
    int choice;
    char input[2];
    do {
        printMenuEditPhoneNombers();
        fgets(input, 2, stdin);
        cleaninput(input);
        choice=atoi(input);
        //scanf("%d", &choice);
        switch (choice) {
            case 1:
                editphoneNumber(contact);
            break;
            case 2:
                addphoneNumbers(contact);
            break;
            case 3:
                deletephoneNumbers(contact);
            break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);
}

//SocialLinks
void printMenuEditSocilaLinks() {
    printf("\nEdit Menu Social links:\n");
    printf("1. Edit Social link\n");
    printf("2. Add Social link\n");
    printf("3. Delete Social link\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void addsociallinks(Contact *contact){
    if(contact->numsocialLinks==MAX_SOCIAL_LINKS)
    {
        printf("\nPhone numbers is full:\n");
        return;
    } else
    {
        printf("Enter social link: ");
        addSocialLink(&contact->socialLinks[contact->numsocialLinks++]);
        printf("Contact update successfully\n");
    }
    
}

void editsociallink(Contact *contact){
    printf("View social links: ");
    for(int i=0;i<contact->numsocialLinks;i++)
    {
        printf("Socil link %d: %s\n", i + 1, contact->socialLinks[i].socialLink);
    }
    int num=0;
    char input[2];
    printf("Enter num socila link for edit (1, 2...) : ");
    //scanf("%d", &num);
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    if (1>num&&num>(contact->numsocialLinks+1)){
        printf("Incorrect input \n");
        return;
    }
    DeleteSocialLink(&contact->socialLinks[num-1]);
    printf("Enter social link for edit: ");
    addSocialLink(&contact->socialLinks[num-1]);

    printf("Contact updated successfully\n");

}

void deletesocialLinks(Contact *contact){
    printf("View Social Links: \n");
    for(int i=0;i<contact->numsocialLinks;i++)
    {
        printf("Social link %d: %s\n", i + 1, contact->socialLinks[i].socialLink);
    }
    int num=0;
    char input[2];
    printf("Enter num socila link for edit (1, 2...) : ");
    //scanf("%d", &num);
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    if (1>num&&num>(contact->numsocialLinks+1)){
        printf("Incorrect input \n");
        return;
    }
    
    DeleteSocialLink(&contact->socialLinks[num-1]);
    if (num-1<contact->numsocialLinks)
    {
        SocialLink SocialLink;
        for (int i = num-1;i<contact->numsocialLinks;i++)
        {
            SocialLink=contact->socialLinks[i];
            contact->socialLinks[i]=contact->socialLinks[i+1];
            contact->socialLinks[i+1]=SocialLink;
        }
        DeleteSocialLink(&contact->socialLinks[contact->numsocialLinks]);
    }
    contact->numsocialLinks--;
        printf("View social links: \n");
    for(int i=0;i<contact->numsocialLinks;i++)
    {
        printf("social link %d: %s\n", i + 1, contact->socialLinks[i].socialLink);
    }
}

void editsocialLinks(Contact *contact)
{
    int choice;
    char input[2];
    do {
        printMenuEditPhoneNombers();
        scanf("%d", &choice);
        fgets(input, 2, stdin);
        cleaninput(input);
        choice=atoi(input);
        switch (choice) {
            case 1:
                editsociallink(contact);
            break;
            case 2:
                addsociallinks(contact);
            break;
            case 3:
                deletesocialLinks(contact);
            break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);
}

//Emails
void printMenuEditemails() {
    printf("\nEdit Menu Emails:\n");
    printf("1. Edit Email\n");
    printf("2. Add Email\n");
    printf("3. Delete Email\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void addemails(Contact *contact){
    if(contact->numemails==MAX_EMAILS)
    {
        printf("\nEmail is full:\n");
        return;
    } else
    {
        printf("Enter Email: ");
        addEmail(&contact->emails[contact->numemails++]);
        printf("Contact added successfully\n");
    }
    
}

void editemail(Contact *contact){
    printf("View email");
    for(int i=0;i<contact->numemails;i++)
    {
        printf("Email %d: %s\n", i + 1, contact->emails[i].email);
    }
    int num=0;
    char input[2];
    printf("Enter num Email for edit (1, 2...) : ");
    //scanf("%d", &num);
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    if (1>num&&num>(contact->numemails+1)){
        printf("Incorrect input \n");
        return;
    }
    DeleteEmail(&contact->emails[num-1]);
    printf("Enter Email for edit: ");
    addEmail(&contact->emails[num-1]);

    printf("Contact updated successfully\n");

}

void deleteemails(Contact *contact){
    printf("View email: \n");
    for(int i=0;i<contact->numemails;i++)
    {
        printf("Phone %d: %s\n", i + 1, contact->emails[i].email);
    }
    int num=0;
    char input[2];
    printf("Enter num email for edit (1, 2...) : ");
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    //scanf("%d", &num);
    if ((1>num)&&(num>(contact->numemails+1))){
        printf("Incorrect input \n");
        return;
    }
    
    DeleteEmail(&contact->emails[num-1]);
    if (num-1<contact->numemails)
    {
        Email Email;
        for (int i = num-1;i<contact->numemails;i++)
        {
            Email=contact->emails[i];
            contact->emails[i]=contact->emails[i+1];
            contact->emails[i+1]=Email;
        }
        DeleteEmail(&contact->emails[contact->numemails]);
    }
    contact->numemails--;
        printf("View email: \n");
    for(int i=0;i<contact->numemails;i++)
    {
        printf("Email %d: %s\n", i + 1, contact->emails[i].email);
    }
}

void editemails(Contact *contact)
{
    int choice;
    char input[2];
    do {
        printMenuEditemails();
        fgets(input, 2, stdin);
        cleaninput(input);
        choice=atoi(input);
        //scanf("%d", &choice);
        switch (choice) {
            case 1:
                editemail(contact);
            break;
            case 2:
                addemails(contact);
            break;
            case 3:
                deleteemails(contact);
            break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);
}

//Contact
void initContact(Contact *contact)
{
    memset(contact->firstName,   '\0', sizeof(char)*(MAX_NAME_LENGTH));
    memset(contact->lastName,    '\0', sizeof(char)*(MAX_NAME_LENGTH));
    memset(contact->jobPosition, '\0', sizeof(char)*(MAX_NAME_LENGTH));
    memset(contact->workplace,   '\0', sizeof(char)*(MAX_NAME_LENGTH));

    for (int i = 0; i<MAX_PHONE_NUMBERS;i++)
    {
    initPhoneNumber(&contact->phoneNumbers[i]);  
    }
    for (int i = 0; i<MAX_SOCIAL_LINKS;i++)
    {
    initSocialLink (&contact->socialLinks[i]);   
    }
    for (int i = 0; i<MAX_EMAILS;i++)
    {
    initEmail(&contact->emails[i]);  
    }
    
    contact->numphoneNumbers=0;
    contact->numsocialLinks =0;
    contact->numemails      =0;
    
}

void createContact(Contact *contact)
{
    
    printf("Enter first name: ");
    fgets(contact->firstName, MAX_NAME_LENGTH, stdin);
    cleaninput(contact->firstName);
    //scanf("%s", contact->firstName);
    printf("Enter last name: ");
    fgets(contact->lastName, MAX_NAME_LENGTH, stdin);
    cleaninput(contact->lastName);
    //scanf("%s", contact->lastName);
    printf("Enter job position: ");
    fgets(contact->jobPosition, MAX_NAME_LENGTH, stdin);
    cleaninput(contact->jobPosition);
    //scanf("%s", contact->jobPosition);
    printf("Enter workplace: ");
    fgets(contact->workplace, MAX_NAME_LENGTH, stdin);
    cleaninput(contact->workplace);
    //scanf("%s", contact->workplace);
   
   
    

    char input[2];
    int num=0;
    printf("enter the number of phone numbers from 0 to 3: ");
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    
    while(num > MAX_PHONE_NUMBERS || num<0){
    printf("incorrect input!!!");  
    printf("enter the number of phone numbers from 0 to 3: ");  
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    }
    if (num>0){
    for(int i=0;i<num;i++){
        printf("enter the phone number %d: ", i+1);  
        addPhoneNumber(&contact->phoneNumbers[i]);
    }}
    contact->numphoneNumbers=num;

    printf("enter the number of social link from 0 to 3: ");
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    while(num > MAX_SOCIAL_LINKS || num<0){
    printf("incorrect input!!!");   
    printf("enter the number of social link from 0 to 3: "); 
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    }
    
    if (num>0){

    for(int i=0;i<num;i++){
        printf("enter the socialLink %d: ", i+1);  
        addSocialLink(&contact->socialLinks[i]);
    }}
    contact->numsocialLinks=num;

    printf("enter the number of email from 0 to 3: ");
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    
    while(num > MAX_EMAILS || num<0){
    printf("incorrect input!!!");   
    printf("enter the number of social link from 0 to 3: "); 
    fgets(input, 2, stdin);
    cleaninput(input);
    num=atoi(input);
    }
  
    if (num>0){
    for(int i=0;i<num;i++){
        printf("enter the email %d: ", i+1);  
        addEmail(&contact->emails[i]);
    }}
    contact->numemails=num;

}

void DeleteContact(Contact *contact){
    
    memset(contact->firstName,   '\0', sizeof(MAX_NAME_LENGTH));
    memset(contact->lastName,    '\0', sizeof(MAX_NAME_LENGTH));
    memset(contact->jobPosition, '\0', sizeof(MAX_NAME_LENGTH));
    memset(contact->workplace,   '\0', sizeof(MAX_NAME_LENGTH));

    for (int i = 0; i<contact->numphoneNumbers;i++)
    {
    DeletePhoneNumber(contact->phoneNumbers);  
    }
    for (int i = 0; i<contact->numsocialLinks;i++)
    {
    DeleteSocialLink (contact->socialLinks);   
    }
    for (int i = 0; i<contact->numemails ;i++)
    {
    DeleteEmail(contact->emails);  
    }
    
    contact->numphoneNumbers=0;
    contact->numsocialLinks =0;
    contact->numemails      =0;
}

void editContact(Contact *contact){
    int choice;
    char input[2];
    do {
        printMenuEditContact();
        fgets(input, 2, stdin);
        cleaninput(input);
        choice=atoi(input);
        switch (choice) {
            case 1:
                memset(contact->firstName,   '\0', sizeof(MAX_NAME_LENGTH));
                printf("Enter first name: ");
                //scanf("%s", contact->firstName);
                fgets(contact->firstName, MAX_NAME_LENGTH, stdin);
                cleaninput(contact->firstName);
            break;
            case 2:
                memset(contact->lastName,    '\0', sizeof(MAX_NAME_LENGTH));
                printf("Enter last name: ");
                fgets(contact->lastName, MAX_NAME_LENGTH, stdin);
                cleaninput(contact->lastName);
            break;
            case 3:
                memset(contact->jobPosition, '\0', sizeof(MAX_NAME_LENGTH));
                printf("Enter job position: ");
                fgets(contact->jobPosition, MAX_NAME_LENGTH, stdin);
                cleaninput(contact->jobPosition);
            break;
            case 4:
                memset(contact->workplace,   '\0', sizeof(MAX_NAME_LENGTH));
                printf("Enter workplace: ");
                fgets(contact->workplace, MAX_NAME_LENGTH, stdin);
                cleaninput(contact->workplace);
            break;
            case 5:
                editphoneNumbers(contact);
            break;
            case 6:
                editsocialLinks(contact);
            break;
            case 7:
                editemails(contact);
            break;
            case 8:
                printf("Exiting...\n");
            break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 8);
}

