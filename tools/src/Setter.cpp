#include "Setter.h"


Setter::Setter() {
    cv::FileStorage SettingFile;
    assert(SettingFile.open("../tools/parameters_lists/para.yaml",cv::FileStorage::READ));
    SettingFile["CNN"]>>cnnPara.CNN;
    SettingFile["Model"]>>Model;
    SettingFile["isUsingCNN"]>>cnnPara.isUsingCNN;
    SettingFile["CalibrationResultPath"]>>calibratorPara.CalibrationResultPath;
    SettingFile["CalibrationPointPath"]>>calibratorPara.CalibrationPointPath;
    SettingFile["SAVE_IMAGE_PATH"]>>calibratorPara.SAVE_IMAGE_PATH;
    SettingFile["whoIam"]>>armourPara.whoIam;
    SettingFile["CornersMatrix"]>>calibratorPara.CornersMatrix;
    SettingFile["Rectangle"]>>calibratorPara.Rectangle;
    SettingFile["SerialPath"]>>serialPort.SerialPath;

    SettingFile["BigBuffLength"]>>allsize[BUFFER].len;                //竖
    SettingFile["BigBuffHeight"]>>allsize[BUFFER].width;              //横
    SettingFile["NormalArmorLength"]>> allsize[SMALLARMOUR].len;
    SettingFile["NormalArmorWidth"]>>allsize[SMALLARMOUR].width;
    SettingFile["LargerArmorWidth"]>>allsize[LARGEARMOUR].width;
    SettingFile["LargerArmorLength"]>>allsize[LARGEARMOUR].len;

    cv::FileStorage fs("../tools/parameters_lists/cr.yaml", cv::FileStorage::READ);
    fs["camera_Matrix"]>>cameraDistortion.CameraMatrix;
    fs["dist_Coeffs"]>>cameraDistortion.DistCoeffs;//读取相机参数和畸变系数
    fs.release();

    SettingFile["isShowOriginImage"]>>armourPara.isShowOriginImage;
    SettingFile["isShowGrayImage"]>>armourPara.isShowGrayImage;
    SettingFile["isShowFinallyImage"]>>armourPara.isShowFinallyImage;

    SettingFile["ModelY"]>>AnglePModelY.transMat;
    SettingFile["AdynamParams"]>>AnglePModelY.dynamParams;
    SettingFile["AmeasureParams"]>>AnglePModelY.measureParams;
    SettingFile["ModelP"]>>AnglePModelP.transMat;
    SettingFile["AdynamParams"]>>AnglePModelP.dynamParams;
    SettingFile["AmeasureParams"]>>AnglePModelP.measureParams;
    SettingFile["ModelF"]>>AnglePModelF.transMat;
    SettingFile["AdynamParams"]>>AnglePModelF.dynamParams;
    SettingFile["AmeasureParams"]>>AnglePModelF.measureParams;
    SettingFile.release();
}
