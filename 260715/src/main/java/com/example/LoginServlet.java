package com.example;

import java.io.IOException;
import java.time.LocalDate;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

/**
 * 로그인 -> Session에 사용자 저장, Cookie에 마지막 방문일 저장
 */
@WebServlet("/login")
public class LoginServlet extends HttpServlet {

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        String username = req.getParameter("username");

        // 1) Session: 서버 메모리에 저장, 로그인 상태 유지용
        HttpSession session = req.getSession();
        session.setAttribute("user", username);
        session.setMaxInactiveInterval(30 * 60); // 30분

        // 2) Cookie: 클라이언트(브라우저)에 저장, 방문 기록용
        Cookie cookie = new Cookie("lastVisit", LocalDate.now().toString());
        cookie.setMaxAge(60 * 60 * 24 * 7); // 7일
        cookie.setPath("/myapp");
        resp.addCookie(cookie);

        resp.sendRedirect("welcome.jsp");
    }
}
