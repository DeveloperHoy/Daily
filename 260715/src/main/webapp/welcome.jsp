<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="jakarta.tags.core" %>
<!DOCTYPE html>
<html>
<head><meta charset="UTF-8"><title>Welcome</title></head>
<body>
    <c:if test="${empty sessionScope.user}">
        <p>로그인이 필요합니다. <a href="index.jsp">로그인 하러 가기</a></p>
    </c:if>

    <c:if test="${not empty sessionScope.user}">
        <h1>환영합니다, ${sessionScope.user}님!</h1>

        <%
            // 쿠키 읽기는 스크립틀릿으로 (JSTL 태그로도 가능하지만 학습 목적상 자바 코드 그대로)
            jakarta.servlet.http.Cookie[] cookies = request.getCookies();
            String lastVisit = "기록 없음";
            if (cookies != null) {
                for (jakarta.servlet.http.Cookie cookie : cookies) {
                    if ("lastVisit".equals(cookie.getName())) {
                        lastVisit = cookie.getValue();
                    }
                }
            }
        %>
        <p>마지막 방문 쿠키 값: <%= lastVisit %></p>
    </c:if>

    <a href="index.jsp">처음으로</a>
</body>
</html>
