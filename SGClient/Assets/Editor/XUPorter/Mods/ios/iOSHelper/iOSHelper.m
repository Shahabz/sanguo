#import "iOSHelper.h"

@implementation iOSHelper

 
char *_getCountry()
{
   	NSLocale *locale = [NSLocale currentLocale];
    NSString *countrycode = [locale localeIdentifier];
    
    const char *country = [countrycode UTF8String];
    
    char *back = (char*)malloc(countrycode.length + 1);
    char *back2 = back;
    
    for (int i = 0;i<countrycode.length; i++) 
    {
        *back2 = country[i];
        back2++;
    }
    *back2 = '\0';
    return back;
}

char *_getLanguage()
{	
		NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSArray * allLanguages = [defaults objectForKey:@"AppleLanguages"];
    NSString * currentLanguage = [allLanguages objectAtIndex:0];
    	
    const char *language = [currentLanguage UTF8String];
    
    char *back = (char*)malloc(currentLanguage.length + 1);
    char *back2 = back;
    
    for (int i = 0;i<currentLanguage.length; i++) 
    {
        *back2 = language[i];
        back2++;
    }
    *back2 = '\0';
    return back;
}

@end
 
