
@interface IOSAlbumCameraController : UIViewController<UIImagePickerControllerDelegate,UINavigationControllerDelegate>
{
@public
    NSString *Photo_pathfile;
    NSString *Photo_name;
    int Photo_picsize;
    float Photo_quality;
}
@end
