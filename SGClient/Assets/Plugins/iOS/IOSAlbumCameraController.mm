#import "IOSAlbumCameraController.h"

@implementation IOSAlbumCameraController

- (void)showPicker:
(UIImagePickerControllerSourceType)type
     allowsEditing:(BOOL)flag
{
    NSLog(@" --- showPicker!!");
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.sourceType = type;
    picker.allowsEditing = flag;
    
    [self presentViewController:picker animated:YES completion:nil];
}

// 打开相册后选择照片时的响应方法
- (void)imagePickerController:(UIImagePickerController*)picker didFinishPickingMediaWithInfo:(NSDictionary*)info
{
    NSLog(@" --- imagePickerController didFinishPickingMediaWithInfo!!");
    // Grab the image and write it to disk
    UIImage *image;
    UIImage *image2;
    image = [info objectForKey:UIImagePickerControllerEditedImage];
    UIGraphicsBeginImageContext(CGSizeMake(Photo_picsize,Photo_picsize));
    [image drawInRect:CGRectMake(0, 0, Photo_picsize, Photo_picsize)];
    image2 = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    // 得到了image，然后用你的函数传回u3d
    NSData *imgData;
    imgData= UIImageJPEGRepresentation(image2, Photo_quality);
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager createFileAtPath:Photo_pathfile contents:imgData attributes:nil];

	UnitySendMessage( "PhotoManagerCallbackEntry", "messgae", Photo_name.UTF8String );
    
    // 关闭相册
    [picker dismissViewControllerAnimated:YES completion:nil];
}

// 打开相册后点击“取消”的响应
- (void)imagePickerControllerDidCancel:(UIImagePickerController*)picker
{
//    NSLog(@" --- imagePickerControllerDidCancel !!");
    [self dismissViewControllerAnimated:YES completion:nil];
}


@end

//------------- called by unity -----begin-----------------
#if defined (__cplusplus)
extern "C" {
#endif
    
    void openPhoto( int op, const char* pathfile, const char* name, int picsize, int quality )
    {
        //UnityPause( true );
        if( op == 0 )
        {
            if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
            {
                IOSAlbumCameraController * app = [[IOSAlbumCameraController alloc] init];
                //app->Photo_pathfile = [[NSString alloc] initWithUTF8String:pathfile];
                app->Photo_pathfile = [NSString stringWithFormat:@"%s/%s", pathfile, name ];
                app->Photo_name = [[NSString alloc] initWithUTF8String:name];
                app->Photo_picsize = picsize;
                app->Photo_quality = (float)quality/100.0;

                UIViewController *vc = UnityGetGLViewController();
                [vc.view addSubview: app.view];
                
                [app showPicker:UIImagePickerControllerSourceTypeCamera allowsEditing:YES];
            }
        }
        else if( op == 1 )
        {
            if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
            {
                IOSAlbumCameraController * app = [[IOSAlbumCameraController alloc] init];
                //app->Photo_pathfile = [[NSString alloc] initWithUTF8String:pathfile];
                app->Photo_pathfile = [NSString stringWithFormat:@"%s/%s", pathfile, name ];
                app->Photo_name = [[NSString alloc] initWithUTF8String:name];
                app->Photo_picsize = picsize;
                app->Photo_quality = (float)quality/100.0;

                UIViewController *vc = UnityGetGLViewController();
                [vc.view addSubview: app.view];
                
                [app showPicker:UIImagePickerControllerSourceTypePhotoLibrary allowsEditing:YES];
            }
        }
        else
        {
            if([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeSavedPhotosAlbum])
            {
                IOSAlbumCameraController * app = [[IOSAlbumCameraController alloc] init];
                //app->Photo_pathfile = [[NSString alloc] initWithUTF8String:pathfile];
                app->Photo_pathfile = [NSString stringWithFormat:@"%s/%s", pathfile, name ];
                app->Photo_name = [[NSString alloc] initWithUTF8String:name];
                app->Photo_picsize = picsize;
                app->Photo_quality = (float)quality/100.0;
                app->Photo_pathfile = [NSString stringWithFormat:@"%s/%s", pathfile, name ];

                UIViewController *vc = UnityGetGLViewController();
                [vc.view addSubview: app.view];
                
                [app showPicker:UIImagePickerControllerSourceTypeSavedPhotosAlbum allowsEditing:YES];
            }
        }
        
    }
    
#if defined (__cplusplus)
}
#endif
//------------- called by unity -----end-----------------