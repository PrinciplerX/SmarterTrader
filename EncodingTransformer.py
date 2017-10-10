import os
import chardet

def strExpand(source, length, char):
    currLen = len(source)
    if currLen < length:
        source = source + char[0] * (length - currLen)
    return source

def transformEncoding(targetEncode, workDir, fileExtension, recursively):
    for parentPath, dirList, fileList in os.walk(workDir):
        # 处理一个文件夹中的文件
        print("开始处理文件夹%s" % parentPath)
        
        for fileName in fileList:
            if fileName.endswith(fileExtension):
                filePath = os.path.join(parentPath, fileName)
                file = open(filePath, 'rb')
                content = file.read()
                file.close()

                file = open(filePath, 'wb')
                
                fileEncode = chardet.detect(content)['encoding']

                if(fileEncode != None):
                    content = content.decode(fileEncode, 'ignore')
                    content = content.encode(targetEncode, 'ignore')

                    file.write(content)
                    print("文件%s\t原编码方式为%s\t，已被转码为%s" % (strExpand(fileName, 25, ' '),
                                                                    strExpand(fileEncode, 10, ' '),
                                                                    strExpand(targetEncode, 10, ' ')))
                else:
                    print("文件%s\t编码方式无法识别，略过。" % strExpand(fileName, 25, ' '))
                    
                file.close()

        # 递归处理子文件夹中的文件        
        if recursively == True:
            for dirName in dirList:
                childDirPath = os.path.join(parentPath, dirName)
                transformEncoding(targetEncode, childDirPath, fileExtension, True)

if __name__ == '__main__':
    print("---------------文件批量转码工具---------------")
    workDir = input("工作目录：")
    fileExtension = input("要转码的文件扩展名（.xxx）：")
    targetEncode = input("目标编码：")
    recursivelyProcess = input("是否递归查找（y/n）：")
    recursively = (recursivelyProcess == 'y') or (recursivelyProcess == 'Y')

    transformEncoding(targetEncode, workDir, fileExtension, recursively)
    
    input("Press any key to continue . . .")
    



                
    
    
